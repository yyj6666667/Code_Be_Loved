/**
 * 哈哈，终于修成正果
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cmath>
#include <iostream>
#include <algorithm>

namespace py = pybind11;
using namespace std;

void softmax_regression_epoch_cpp(const float *X, const unsigned char *y,
								  float *theta, size_t m, size_t n, size_t k,
								  float lr, size_t batch)
{
    float *logits = new float[batch * k];
    float *probs  = new float[batch * k];
    float *grad_theta = new float[n * k]; //gradient for theta

    for (int start = 0; start < m; start += batch) {
        int current_batch_size = std::min(batch, m - start);

        //step 1.compute logits for current batch (logits = X_batch @ theta)
        memset(logits, 0, sizeof(float) * current_batch_size * k);
        for (int i = 0; i < current_batch_size; i++) {
            for (int j = 0; j < k; j++){
                for (int p = 0; p < n; p++) {
                    logits[i * k + j] += X[(start + i) * n + p] * theta[p * k + j]; 
                }
            }
        }
        //step 2.apply softmax to logits for probabilities
        for (int i = 0; i < current_batch_size; i++) {
            float sum_exp = 0.0;
            //预防梯度爆炸
            int   offset = i * k;
            float max_logits_per_row = *std::max_element(logits + offset, logits + offset + k);
            for (int j = 0; j < k ; j++) {
                logits[i * k + j] -= max_logits_per_row;
            }
            for (int j = 0; j < k; j++) {
                probs[i * k + j] = std::exp(logits[i * k + j]);
                sum_exp += probs[i*k + j];
            }
            for (int j = 0; j < k; j++) {
                probs[i * k + j] /= sum_exp;
            }
        }

        //step 3.compute grad for theta
        //这里是借鉴了rickyyu的写法，说实话，还没有太消化
        memset(grad_theta, 0, sizeof(float) * n * k);
        for (int i = 0; i < current_batch_size; i++) {
            for (int j = 0; j < k; j++) {
                float error = probs[i * k + j] - (j == y[start + i] ? 1.0f : 0.0f);
                for (int p = 0; p < n; p++) {
                    grad_theta[p * k + j] += X[(start + i) * n + p] * error;
                }
            }
        }

        //step 4: update theta using grad_theta
        for (int p = 0; p < n; p++) {
            for (int j = 0; j < k; ++j) {
                theta[p * k + j] -= lr * grad_theta[p * k + j] / current_batch_size;
            }
        }
    }

    delete[] logits;
    delete[] probs;
    delete[] grad_theta;
}

//以下是为了嵌入python里面：
PYBIND11_MODULE(simple_ml_ext, m) {
    m.def("softmax_regression_epoch_cpp",
    //解析python，一项参数的开始
    []( py::array_t<float, py::array::c_style> X,
        py::array_t<unsigned char, py::array::c_style> y,
        py::array_t<float, py::array::c_style> theta,
        float lr, int batch
      ) {
                //解析python into cpp form
                softmax_regression_epoch_cpp(
                    static_cast<const float*>(X.request().ptr),
                    static_cast<const unsigned char*>(y.request().ptr),
                    static_cast<float*>(theta.request().ptr),
                    X.request().shape[0],
                    X.request().shape[1],
                    theta.request().shape[1],
                    lr,
                    batch
                );
        },
    //本项参数结束
    py::arg("X"), 
    py::arg("y"), 
    py::arg("theta"), 
    py::arg("lr"), 
    py::arg("batch")
    );
}