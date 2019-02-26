#include "Math/gfp.h"
#include "Math/gf2n.h"
#include "Networking/sockets.h"
#include "Tools/int.h"
#include "Math/Setup.h"
#include "Auth/fake-stuff.h"

#include <vector>
#include <sodium.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

namespace common {

    // Send the private inputs masked with a random value.
    // Receive shares of a preprocessed triple from each SPDZ engine, combine and check the triples are valid.
    // Add the private input values to triple[0] and send to each spdz engine.
    void send_private_inputs(vector<gfp>& values, vector<int>& sockets, int num_parties)
    {
        int num_inputs = values.size();
        octetStream os;
        vector <vector<gfp>> triples(num_inputs, vector<gfp>(3));
        vector<gfp> triple_shares(3);

        // Receive num_inputs triples from SPDZ
        for (int j = 0; j < num_parties; j++)
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
        for (int j = 0; j < num_parties; j++)
            os.Send(sockets[j]);
    }

    // Assumes that Scripts/setup-online.sh has been run to compute prime
    void init_fields(const string& dir_prefix) {
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
    gfp receive_result(vector<int>& sockets, int num_parties)
    {
        vector<gfp> output_values(3);
        octetStream os;
        for (int i = 0; i < num_parties; i++)
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
}