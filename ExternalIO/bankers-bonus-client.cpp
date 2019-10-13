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

// Send the private inputs masked with a random value.
// Receive shares of a preprocessed triple from each SPDZ engine, combine and check the triples are valid.
// Add the private input value to triple[0] and send to each spdz engine.
void send_private_inputs(vector<gfp>& values, vector<int>& sockets, int nparties)
{
    int num_inputs = values.size();
    octetStream os;
    vector< vector<gfp> > triples(num_inputs, vector<gfp>(3));
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
    // Send inputs + triple[0], so SPDZ can compute shares of each value
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
void initialise_fields(const string& dir_prefix)
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

vector<string> get_party_hosts(string hosts_filename)
{
    vector<string> hosts;

    ifstream infile(hosts_filename);
    std::string line;
    while (getline(infile, line))
    {
        istringstream iss(line);
        string val;
        while (iss >> val)
        {
            hosts.push_back(val);
        }
    }

    return hosts;
}

int main(int argc, char** argv)
{
    int my_client_id;
    int nparties;
    int action;
    int finish;
    string host_file_name = "";
    int port_base = 14000;

    if (argc < 5) {
        cout << "Usage is bankers-bonus-client <client identifier> <number of spdz parties> "
           << "<action (0 for chisq simulation, 1 for other test simuation)> " 
           << "<finish (0 false, 1 true)> <optional host name, default localhost> "
           << "<optional spdz party port base number, default 14000>" << endl;
        exit(0);
    }

    my_client_id = atoi(argv[1]);
    nparties = atoi(argv[2]);
    action = atoi(argv[3]);
    finish = atoi(argv[4]);
    if (argc > 5)
        host_file_name = argv[5];
    
    if (argc > 6)
        port_base = atoi(argv[6]);

    // init static gfp
    string prep_data_prefix = get_prep_dir(nparties, 128, 40);
    initialise_fields(prep_data_prefix);

    vector<string> party_hosts = get_party_hosts(host_file_name);
    // Setup connections from this client to each party socket
    vector<int> sockets(nparties);
    for (int i = 0; i < nparties; i++)
    {
        set_up_client_socket(sockets[i], party_hosts[i].c_str(), port_base + i);
    }
    cout << "Finish setup socket connections to SPDZ engines." << endl;


    // Map inputs into gfp 
    vector<gfp> input_values_gfp(3);
    input_values_gfp[0].assign(my_client_id);
    input_values_gfp[1].assign(action);
    input_values_gfp[2].assign(finish);    

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
