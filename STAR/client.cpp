/*
 * Provides a client to *compute tests* using star_program.mpc program.
 * 
 * NOTE [WARNING]: No communication security is used! See bankers-bonus-commsec.cpp for 
 * example of implementing communication security. 
 * 
 * Usage:
 *  ./client.x 2 <num-spdz-parties> <dataset id> <test type> <num attr> <attr1> <attr2>...<attrN> <finish>
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

int main(int argc, char** argv) {
  
    int num_parties;
    int dataset_id;
    int finish;
    int num_attr;
    int port_base = 14000;
    string test_type;
    string host_file = "";
    string data_file_name = "";

    if (argc < 8) {
        cout << "Usage: ./client <number of spdz parties>  <dataset id> <test type (--ttest | --pearson | --chisq)>"
           <<  "<num attributes (min=2, max=20)> <attribute (0|1)> < attribute (0|1)> ... <attribute (0|1)>"
           << "<finish (0=false, 1=true)> [optional host name, (default=localhost]> "
           << "[optional spdz party port base number (default=14000)]>" << endl;
        exit(0);
    }

    // required args
    num_parties = atoi(argv[1]);
    dataset_id = atoi(argv[2]);
    test_type = argv[3];
    num_attr = atoi(argv[4]);
    finish = atoi(argv[5 + num_attr]);

    // optional args
    if (argc > 6 + num_attr)
        host_file = argv[6 + num_attr];
    if (argc > 7 + num_attr)
        port_base = atoi(argv[7 + num_attr]);

    // parse party ip addresses 
    std::vector<string> host_names;
    std::ifstream infile(host_file);
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        host_names.push_back(line);
    }

    // print inputs 
    cout << "Received input:" << "\n"
        << "Number of Parties:  " << to_string(num_parties) << "\n"
        << "Test type:          " << test_type << "\n"
        << "Num Attributes:     " << to_string(num_attr)
        << "Hosts file:     " << host_file << endl;


    vector<int> attributes;
    for (int i = 0; i < num_attr; i++) {
        attributes.push_back(atoi(argv[5 + i]));
        cout << "   attr_" << to_string(i) << ":  " << to_string(attributes[i]) << endl;
    }

    cout << "Quit? " << (finish == 0 ? "FALSE" : "TRUE") << endl;

    // init static gfp
    string prep_data_prefix = get_prep_dir(num_parties, PRIME_BITS, FIELD_BITS);
    common::init_fields(prep_data_prefix);

    // setup connections from this client to each party socket
    vector<int> sockets(num_parties);
    for (int i = 0; i < num_parties; i++)  {
        set_up_client_socket(sockets[i], host_names[i].c_str(), port_base + i);
    }

    cout << "Finished setup of socket connections to SPDZ engines." << endl;

    // to feed the input to the SPDZ engine, we need to flatten the matrix
    // into an array of gfp. Array size = cols*rows + extra (for metadata)
    vector<gfp> input_values_gfp(MAX_NUM_EXTRA_PARAMS);

    int test_type_int = -1;
    if (!test_type.compare(TEST_TYPE_TTEST)) {
        test_type_int = 1;
    } else if (!test_type.compare(TEST_TYPE_PEARSON)) {
        test_type_int = 2;
    } else if (!test_type.compare(TEST_TYPE_CHISQ)) {
        test_type_int = 3;
    } else if (!test_type.compare(TEST_TYPE_FTEST)) {
        test_type_int = 4;
    }
 
    if (test_type_int == -1) {
        cout << "Invalid test specified!" << endl;
        
        for (unsigned int i = 0; i < sockets.size(); i++)
            close_client_socket(sockets[i]);

        return 1;
    }

    cout << "Client selected action = " << to_string(test_type_int) << endl;

    // only relevant inputs when performing other actions
    input_values_gfp[0].assign(test_type_int);     // type of test (1 = ttest, 2 = pearson, 3 = chisq, etc)
    input_values_gfp[1].assign(dataset_id);        // id of the dataset to compute the test over
    input_values_gfp[2].assign(0);                 // dummy
    input_values_gfp[3].assign(0);                 // dummy
    input_values_gfp[4].assign(finish);            // quit the program after running
    input_values_gfp[5].assign(num_attr);          // numer of selected attribute
    
    for (int i = 0; i < num_attr; i++) {
       input_values_gfp[6 + i] = gfp(attributes[i]);
    }
    
    // Run the commputation
    common::send_private_inputs(input_values_gfp, sockets, num_parties);
    cout << "Sent private inputs to each SPDZ engine, waiting for result..." << endl;

    // Get the result back (client_id of winning client)
    gfp result = common::receive_result(sockets, num_parties);

    cout << "Response: " << result << endl;
    
    for (unsigned int i = 0; i < sockets.size(); i++)
        close_client_socket(sockets[i]);

    return 0;
}