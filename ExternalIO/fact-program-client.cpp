/*
 * Provides a client to fact_program.mpc program to compute statistical tests.
 * Up to 8 clients can connect to the SPDZ engines running 
 * the fact_program.mpc program and provide input data (up 10k datapoints per client).
 *
 * Each connecting client:
 * - sends a unique id to identify the client
 * - sends an integer indicating input size <= 10k
 * - an array of up to 10k integer values
 *
 * The result is returned authenticated with a share of a random value:
 * - share of test result [y]
 * - share of random value [r]
 * - share of test result * random value [w]
 *   test result is valid iff ∑ [y] * ∑ [r] = ∑ [w]
 * 
 * No communications security is used. 
 *
 * To run with 2 parties / SPDZ engines:
 *   ./Scripts/setup-online.sh to create triple shares for each party (spdz engine).
 *   ./compile.py fact_program
 *   ./Scripts/run-online fact_program    to run the engines.
 *
 *   ./fact_program_client.x 123 <filename> <num spdz parties> <test id> 0
 *   ./fact_program_client.x 456 <filename> <num spdz parties> <test id> 0
 *   ./fact_program_client.x 789 <filename> <num spdz parties> <test id> 1
 *
 */

#include "Math/gfp.h"
#include "Math/gf2n.h"
#include "Networking/sockets.h"
#include "Tools/int.h"
#include "Math/Setup.h"
#include "Auth/fake-stuff.h"

#include <sodium.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdlib.h>

// Send the private inputs masked with a random value.
// Receive shares of a preprocessed triple from each SPDZ engine, combine and check the triples are valid.
// Add the private input values to triple[0] and send to each spdz engine.
void send_private_inputs(vector<gfp>& values, vector<int>& sockets, int nparties)
{
    int num_inputs = values.size();
    octetStream os;
    vector <vector<gfp>> triples(num_inputs, vector<gfp>(3));
    vector<gfp> triple_shares(3);

    // Receive num_inputs triples from SPDZ
    for (int j = 0; j < nparties; j++)
    {
        os.reset_write_head();
        os.Receive(sockets[j]);

        for (int j = 0; j < num_inputs; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                triple_shares[k].unpack(os);
                triples[j][k] += triple_shares[k];
            }
        }
    }

    // Check triple relations (is a party cheating?)
    for (int i = 0; i < num_inputs; i++)
    {
        if (triples[i][0] * triples[i][1] != triples[i][2])
        {
            cerr << "Incorrect triple at " << i << ", aborting\n";
            exit(1);
        }
    }
    
    // Send input + triple[0] for each input, so SPDZ can compute shares of each value
    os.reset_write_head();
    for (int i = 0; i < num_inputs; i++)
    {
        gfp y = values[i] + triples[i][0];
        y.pack(os);
    }
    for (int j = 0; j < nparties; j++)
        os.Send(sockets[j]);
}

// Assumes that Scripts/setup-online.sh has been run to compute prime
void init_fields(const string& dir_prefix)
{
  int lg2;
  bigint p;

  string filename = dir_prefix + "Params-Data";
  cout << "loading params from: " << filename << endl;

  ifstream inpf(filename.c_str());
  if (inpf.fail()) { throw file_error(filename.c_str()); }
  inpf >> p;
  inpf >> lg2;

  inpf.close();

  gfp::init_field(p);
  gf2n::init_field(lg2);
}


// Receive shares of the result and sum together.
// Also receive authenticating values.
gfp receive_result(vector<int>& sockets, int nparties)
{
    vector<gfp> output_values(3);
    octetStream os;
    for (int i = 0; i < nparties; i++)
    {
        os.reset_write_head();
        os.Receive(sockets[i]);
        for (unsigned int j = 0; j < 3; j++)
        {
            gfp value;
            value.unpack(os);
            output_values[j] += value;            
        }
    }

    if (output_values[0] * output_values[1] != output_values[2])
    {
        cerr << "Unable to authenticate output value as correct, aborting." << endl;
        exit(1);
    }
    return output_values[0];
}
 

int main(int argc, char** argv)
{
    int my_client_id;
    int nparties;
    int finish;
    int port_base = 14000;
    string host_name = "localhost";
    string data_file_name = "";

    if (argc < 5) {
        cout << "Usage is fact_program <client identifier> <file name> <number of spdz parties> "
           << "<finish (0=false, 1=true)> [optional host name, (default=localhost]> "
           << "[optional spdz party port base number (default=14000)]>" << endl;
        exit(0);
    }

    // required args
    my_client_id = atoi(argv[1]);
    data_file_name = argv[2];
    nparties = atoi(argv[3]);
    finish = atoi(argv[4]);

    cout << "***Input data***" << "\n"
        << "Client ID:         " << to_string(my_client_id) << "\n"
        << "Filename:          " << data_file_name << "\n"
        << "Number of Parties: " << to_string(nparties) << "\n"
        << "Finished?          " << (finish == 0 ? "FALSE" : "TRUE") << "\n" 
        << endl;

    // optional args
    if (argc > 5)
        host_name = argv[4];
    if (argc > 6)
        port_base = atoi(argv[5]);

    // init static gfp
    string prep_data_prefix = get_prep_dir(nparties, 512, 40);
    init_fields(prep_data_prefix);

    // parse the data
    // and map inputs into gfp vectors
    vector<gfp> input_x;
	vector<gfp> input_y;
    int x, y;
    ifstream infile(data_file_name);
    while (infile >> x >> y) {
        input_x.push_back(gfp(x));
        input_y.push_back(gfp(y));   
    }

    // setup connections from this client to each party socket
    vector<int> sockets(nparties);
    for (int i = 0; i < nparties; i++)  {
        set_up_client_socket(sockets[i], host_name.c_str(), port_base + i);
    }
    cout << "Finished setup of socket connections to SPDZ engines." << endl;


    int num_extra = 2; // number of extra data
    int db_size = input_x.size(); // inputs must be the same!

    vector<gfp> input_values_gfp(2*db_size + num_extra); // size + num extra inpits
    input_values_gfp[0].assign(db_size);   
    input_values_gfp[1].assign(finish);    

    for (int i = num_extra; i < db_size + num_extra; i++) {
        input_values_gfp[i].assign(input_x[i-num_extra]);
    }
    for (int i = db_size + num_extra; i < 2*db_size + num_extra; i++) {
        input_values_gfp[i].assign(input_y[i - db_size - num_extra]);
    }

    // Run the commputation
    send_private_inputs(input_values_gfp, sockets, nparties);
    cout << "Sent private inputs to each SPDZ engine, waiting for result..." << endl;

    // Get the result back (client_id of winning client)
    gfp result = receive_result(sockets, nparties);

    cout << "Winning client id is : " << result << endl;
    
    for (unsigned int i = 0; i < sockets.size(); i++)
        close_client_socket(sockets[i]);

    return 0;
}
