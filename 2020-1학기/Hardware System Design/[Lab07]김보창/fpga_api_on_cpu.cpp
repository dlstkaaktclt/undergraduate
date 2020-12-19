#include "fpga_api.h"
#include <stdio.h>
#include <iostream>
#include <cstring>

using namespace std;

#define min(x, y) (((x) < (y)) ? (x) : (y))

FPGA::FPGA(off_t data_addr, off_t output_addr, int m_size, int v_size)
{
  m_size_ = m_size;
  v_size_ = v_size;
  data_size_ = (m_size_ + 1) * v_size_; // fpga bram data size

  output_ = new unsigned int[m_size_]; // use output_ as tempolar output
  data_ = new float[data_size_];

  num_block_call_ = 0;
}
FPGA::~FPGA()
{
  delete[] output_;
  delete[] data_;
}

float *FPGA::matrix(void)
{
  return data_ + v_size_;
}

float *FPGA::vector(void)
{
  return data_;
}

void FPGA::reset(void)
{
  num_block_call_ = 0;
}

int FPGA::num_block_call(void)
{
  return num_block_call_;
}

const float *FPGA::blockMV()
{
  num_block_call_ += 1;

  // cpu version
  float *vec = this->vector();
  float *mat = this->matrix();
  float *out = reinterpret_cast<float *>(output_);

  for (int i = 0; i < m_size_; ++i)
  {
    out[i] = 0;
    for (int j = 0; j < v_size_; ++j)
      out[i] += vec[j] * mat[v_size_ * i + j];
  }

  for (int i = 0; i < m_size_; ++i)
    data_[i] = out[i];

  return data_;
}

// m_size = block row
// v_size = block col
// num_output = matrix row
// num_input = matrix col
void FPGA::largeMV(const float* large_mat, const float* input, float* output, int num_input, int num_output)
{
	float* vec = this->vector();
	float* mat = this->matrix();

	// 0) Initialize output vector		
	for(int i = 0; i < num_output; ++i)
	{
		output[i] = 0;
	}

	for(int i = 0; i < num_output; i += m_size_)
	{
		for(int j = 0; j < num_input; j += v_size_)
		{			
			// 0) Initialize input vector		
			int block_row = min(m_size_, num_output-i);
			int block_col = min(v_size_, num_input-j);

			// !) Assign a vector
			/* IMPLEMENT */
            memset(vec, 0, sizeof(float) * v_size_);
			memcpy(vec, (input + j), sizeof(float) * block_col);

			// 2) Assign a matrix
			/* IMPLEMENT */
            memset(mat, 0, sizeof(float) * m_size_ * v_size_);
            for(int k = 0; k < block_row; k++)
            {
                memcpy((mat + v_size_ * k), (large_mat + (i + k) * num_input + j), sizeof(float) * block_col);
            }

			// 3) Call a function `block_call() to execute MV multiplication
			const float* ret = this->blockMV();

			// 4) Accumulate intermediate results
			for(int row = 0; row < block_row; ++row)
			{
				output[i + row] += ret[row];
			}
		} 
	}
}

void FPGA::convLowering(const std::vector<std::vector<std::vector<std::vector<float>>>> &cnn_weights,
                        std::vector<std::vector<float>> &new_weights,
                        const std::vector<std::vector<std::vector<float>>> &inputs,
                        std::vector<std::vector<float>> &new_inputs)
{
  /*
   * Arguments:
   *
   * conv_weights: [conv_channel, input_channel, conv_height, conv_width]
   * new_weights: [conv_channel, input_channel * conv_height * conv_width]
   * inputs: [input_channel, input_height, input_width]
   * new_inputs: [input_channel * conv_height * conv_width, (input_height - conv_height + 1)*(input_width - conv_width + 1) ]
   *
   */

  int conv_channel = cnn_weights.size();
  int input_channel = cnn_weights[0].size();
  int conv_height = cnn_weights[0][0].size();
  int conv_width = cnn_weights[0][0][0].size();
  //int input_channel = inputs.size();
  int input_height = inputs[0].size();
  int input_width = inputs[0][0].size();

  // IMPLEMENT THIS
  // For example,
  // new_weights[0][0] = cnn_weights[0][0][0][0];
  // new_inputs[0][0] = inputs[0][0][0];
    int i,j;
    
    int filter_dim, filter_row, filter_col, ref_row, ref_col, input_dim, input_row, input_col;
    
    int filter_size = input_channel * conv_height * conv_width;
    int block_size = conv_height * conv_width;
    int block_number = (input_height - conv_height + 1) * (input_width - conv_width + 1);

    for(i = 0; i < conv_channel; i++) //fill new_weights
    {
        for(j = 0; j < filter_size; j++)    
        {
            filter_dim = j / block_size; //for filter dimension
            filter_row = (j % block_size) / conv_width; //filter height
            filter_col = (j % block_size) % conv_width; //filter width

            new_weights[i][j] = cnn_weights[i][filter_dim][filter_row][filter_col];
        }
    }

    for(i = 0; i < filter_size; i++)
    {
        for(j = 0; j < block_number; j++) //fill new_inputs
        {
            ref_row = j / (input_width - conv_width + 1);   //block location. inputs[input_channel][ref_row][ref_col] represent left upper point of block.
            ref_col = j % (input_width - conv_width + 1);

            /*
            input_dim = i / block_size;
            input_row = ref_row + (conv_height - 1) - ((i % block_size) / conv_width);
            input_col = ref_col + (conv_width - 1) - ((i % block_size) % conv_width);
            */

            input_dim = i / block_size;
            input_row = ref_row + ((i % block_size) / conv_width);
            input_col = ref_col + ((i % block_size) % conv_width);

            new_inputs[i][j] = inputs[input_dim][input_row][input_col];
        }
    }    

}
