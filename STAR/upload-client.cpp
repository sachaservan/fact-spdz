/*
 * Provides a client to *upload* data to star_program.mpc program.
 * 
 * NOTE [WARNING]: No communication security is used! See bankers-bonus-commsec.cpp for 
 * example of implementing communication security. 
 * 
 * Usage:
 *   ./upload-client.x <data-filename> <num-spdz-parties> <finish>
 */

#include "Math/gfp.h"
#include "Math/gf2n.h"
#include "Networking/sockets.h"
#include "Tools/int.h"
#include "Math/Setup.h"
#include "Auth/fake-stuff.h"

#include "STAR/common.h"
#include "STAR/constants.h"

#include <sodium.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdlib.h>

 
int main(int argc, char** argv)
{
    int num_parties;
    int finish;
    int port_base = 14000;
    string host_file = "";
    string data_file_name = "";

    if (argc < 4) {
        cout << "Usage: ./client <data file name> <number of spdz parties>"
           << "<finish (0=false, 1=true)> [optional host name, (default=localhost]> "
           << "[optional spdz party port base number (default=14000)]>" << endl;
        exit(0);
    }

    // required args
    data_file_name = argv[1];
    num_parties = atoi(argv[2]);
    finish = atoi(argv[3]);

    // optional args
    if (argc > 5)
        host_file = argv[4];
    if (argc > 6)
        port_base = atoi(argv[5]);

    // parse party ip addresses 
    std::vector<string> host_names;
    std::ifstream hostsfile(host_file);
    std::string ip_addr;
    while (std::getline(hostsfile, ip_addr)) {
        std::istringstream iss(ip_addr);
        host_names.push_back(ip_addr);
    }

    // print inputs 
    cout << "Received input:" << "\n"
        << "Filename:     " << data_file_name << "\n"
        << "Num Parties:  " << to_string(num_parties) << "\n"
        << "Finish:       " << (finish == 0 ? "FALSE" : "TRUE") 
        << "Hosts file:     " << host_file << endl;

    // init static gfp
    string prep_data_prefix = get_prep_dir(num_parties, PRIME_BITS, FIELD_BITS);
    common::init_fields(prep_data_prefix);

    // parse the data from file
    // and map inputs into gfp vectors
    vector<vector<int>> input_mat;
    
    ifstream infile(data_file_name);
    std::string line;
    int row_indx = 0;
    while (getline(infile, line))
    {
        istringstream iss(line);
        vector<int> row_vec;
        input_mat.push_back(row_vec);
        int val;
        while (iss >> val) { 
            input_mat[row_indx].push_back(val);
        } 
        row_indx++;
    }
    
    // setup connections from this client to each party socket
    vector<int> sockets(num_parties);
    for (int i = 0; i < num_parties; i++)  {
        set_up_client_socket(sockets[i], host_names[i].c_str(), port_base + i);
    }
    
    cout << "Finished setup of socket connections to SPDZ engines." << endl;

    // simplifying assumption: matrix is filled. i.e., no missing data.
    int num_attr = input_mat[0].size(); // number of columns in the data input
    int num_rows = input_mat.size();    // number of rows in the input columns 

    // to feed the input to the SPDZ engine, we need to flatten the matrix
    // into an array of gfp. Array size = cols*rows + extra (for metadata)
    vector<gfp> input_values_gfp(num_attr*num_rows + MAX_NUM_EXTRA_PARAMS);

    // doing upload action, provide necessary inputs
    input_values_gfp[0].assign(0);           // action_type 0 for UPLOAD
    input_values_gfp[1].assign(0);           // dummy
    input_values_gfp[2].assign(num_rows);    // next we give the number
    input_values_gfp[3].assign(num_attr);    // and the number of attributes
    input_values_gfp[4].assign(finish);      // quit the program
    input_values_gfp[5].assign(0);           // dummy



    // first row is for extra params 
    // starting with second row, we flatten the matrix
    // TODO: not the most intuitive/efficient way to organize the data. Needs thinking + fixing
    for (int i = 0; i < num_attr; i++) {
        for (int j = 0; j < num_rows; j++) {
            input_values_gfp[i*num_rows + j + MAX_NUM_EXTRA_PARAMS].assign(input_mat[j][i]);
        }
    }

    // send off the inputs
    common::send_private_inputs(input_values_gfp, sockets, num_parties);
    cout << "Sent private inputs to each SPDZ engine, waiting for result..." << endl;

    // get back the result (1 if error, 0 otherwise)
    gfp result = common::receive_result(sockets, num_parties);

    cout << "Response: " << result << endl;
    
    // cleanup 
    for (unsigned int i = 0; i < sockets.size(); i++)
        close_client_socket(sockets[i]);

    return 0;
}