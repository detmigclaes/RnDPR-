
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <inttypes.h>
 
using namespace std;




vector <uint32_t> create_random_data(int size)
{


    vector <uint32_t> data(size);
    for (int i = 0; i < size; i++)
    {
        int random_number = (rand() % 1024);
        //int random_number = 512;
        //int random_number = (rand() % 4);
        //random_number = random_number ;
        data[i] = random_number;
    }
    return data;
}

// sinc filter
vector <uint32_t> accumulate(vector <uint32_t> data)
{
    vector <uint32_t> filtered_data(data.size());
    for (int i = 0; i < data.size(); i++)
    {
        if (i == 0)
        {
            filtered_data[i] = data[i];
        }
        else
        {
            filtered_data[i] = data[i] + filtered_data[i-1];
        }
    }
    return filtered_data;
}

vector <uint32_t> downsample(vector <uint32_t> data, int factor)
{
    vector <uint32_t> downsampled_data(data.size()/factor);
    
    for (int i = 0; i < data.size()/factor; i++)
    {
        downsampled_data[i] = data[i*factor];
        
    }
    return downsampled_data;
} 

vector <uint32_t> differentation(vector <uint32_t> data, int factor)
{
    vector <uint32_t> diff_data(data.size());
    // zero pad the front of the data
    data.insert(data.begin(), factor, 0);
    for (int i = factor; i < data.size(); i++)
    {
        //if (i == 0)
        //{
        //    diff_data[i] = data[i];
        //}
        //else
        //{
            diff_data[i-factor] = data[i] - data[(i-factor)];
            
        //}
        
    }
    //cout << "done" << endl;
    return diff_data;
}

vector <uint32_t> MA(vector <uint32_t> data, int factor)
{
    vector <uint32_t> MA_data(data.size());
    // zero pad the front of the data
    data.insert(data.begin(), factor, 0);

    for (int i = factor; i < data.size(); i++)
    {
        uint32_t sum = 0;
        for (int j = 0; j < factor; j++)
        {
            sum += data[i-j];
        }
        MA_data[i-factor] = sum/factor;
    }
    return MA_data;
}


int main()
{
    ofstream myfile;
    // read data from file

     vector <uint32_t> real_data;
    ifstream infile("data_DC_200.txt");
    string line;
    while (getline(infile, line))
    {
        real_data.push_back(stoi(line));
    }
    infile.close(); 

    myfile.open ("sinc_filter_test/real_data.txt");
    for (int i = 0; i < real_data.size(); i++)
    {   myfile << real_data[i] << "\n";   }
    myfile.close();


    vector <uint32_t> random_data = create_random_data(32*1000);
    myfile.open ("sinc_filter_test/random_data.txt");
    for (int i = 0; i < random_data.size(); i++)
    {   myfile << random_data[i] << "\n";   }
    myfile.close();


    // single SINC filter of real data
    vector <uint32_t> accu_real_sinc1 = accumulate(real_data);
    vector <uint32_t> diff_real_sinc1 = differentation(accu_real_sinc1, 32);

    myfile.open ("sinc_filter_test/diff_real_sinc1.txt");
    for (int i = 0; i < diff_real_sinc1.size(); i++)
    {   myfile << diff_real_sinc1[i] << "\n";   }
    myfile.close();


    // triple SINC filter of real data
    vector <uint32_t> accu1_real_sinc3 = accumulate(real_data);
    vector <uint32_t> accu2_real_sinc3 = accumulate(accu1_real_sinc3);
    vector <uint32_t> accu3_real_sinc3 = accumulate(accu2_real_sinc3);
    vector <uint32_t> diff1_real_sinc3 = differentation(accu3_real_sinc3, 8);
    vector <uint32_t> diff2_real_sinc3 = differentation(diff1_real_sinc3, 16);
    vector <uint32_t> diff3_real_sinc3 = differentation(diff2_real_sinc3, 32);

    myfile.open ("sinc_filter_test/diff3_real_sinc3.txt");
    for (int i = 0; i < diff3_real_sinc3.size(); i++)
    {   myfile << diff3_real_sinc3[i] << "\n";   }
    myfile.close();


    // single SINC filter of random data
    vector <uint32_t> accu_random_sinc1 = accumulate(random_data);
    vector <uint32_t> diff_random_sinc1 = differentation(accu_random_sinc1, 32);

    myfile.open ("sinc_filter_test/diff_random_sinc1.txt");
    for (int i = 0; i < diff_random_sinc1.size(); i++)
    {   myfile << diff_random_sinc1[i] << "\n";   }
    myfile.close();


    // triple SINC filter of random data
    vector <uint32_t> accu1_random_sinc3 = accumulate(random_data);
    vector <uint32_t> accu2_random_sinc3 = accumulate(accu1_random_sinc3);
    vector <uint32_t> accu3_random_sinc3 = accumulate(accu2_random_sinc3);
    vector <uint32_t> diff1_random_sinc3 = differentation(accu3_random_sinc3, 32);
    vector <uint32_t> diff2_random_sinc3 = differentation(diff1_random_sinc3, 32);
    vector <uint32_t> diff3_random_sinc3 = differentation(diff2_random_sinc3, 32);

    myfile.open ("sinc_filter_test/diff3_random_sinc3.txt");
    for (int i = 0; i < diff3_random_sinc3.size(); i++)
    {   myfile << diff3_random_sinc3[i] << "\n";   }
    myfile.close();



    // single MA filter of random data
    vector <uint32_t> MA_random_MA1 = MA(random_data, 32);

    myfile.open ("sinc_filter_test/MA_random_MA1.txt");
    for (int i = 0; i < MA_random_MA1.size(); i++)
    {   myfile << MA_random_MA1[i] << "\n";   }
    myfile.close();



    // quarddruble SINC filter of real data
    vector <uint32_t> accu1_real_sinc4 = accumulate(real_data);
    vector <uint32_t> accu2_real_sinc4 = accumulate(accu1_real_sinc4);
    vector <uint32_t> accu3_real_sinc4 = accumulate(accu2_real_sinc4);
    vector <uint32_t> accu4_real_sinc4 = accumulate(accu3_real_sinc4);
    vector <uint32_t> diff1_real_sinc4 = differentation(accu4_real_sinc4, 32);
    vector <uint32_t> diff2_real_sinc4 = differentation(diff1_real_sinc4, 32);
    vector <uint32_t> diff3_real_sinc4 = differentation(diff2_real_sinc4, 32);
    vector <uint32_t> diff4_real_sinc4 = differentation(diff3_real_sinc4, 32);

    myfile.open ("sinc_filter_test/diff4_real_sinc4.txt");
    for (int i = 0; i < diff4_real_sinc4.size(); i++)
    {   myfile << diff4_real_sinc4[i] << "\n";   }
    myfile.close();

    cout << "done!\n";
    

}
