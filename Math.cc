#include "Math.h"
#include <stdexcept>
using namespace std;

Vec3::Vec3(initializer_list<float> data)
{
    vector<float> v = data;
    x = v[0]; y = v[1]; z = v[2];
}

ostream& operator<<(ostream& o, Vec3 v)
{
    return o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

Mat& Mat::set(std::initializer_list<float> _data)
{
    if (_data.size() != data.size())
        throw std::length_error("invalid size of initializer_list to assign matrix");
    data = _data;
    return *this;
}

ostream& operator<<(ostream& o, Mat A)
{
    o << "[";
    for (int i = 0; i < A.row; i++) {
        if (i != 0) o << "; ";
        for (int j = 0; j < A.col; j++) {
            if (j != 0) o << ", ";
            o << A(i,j);
        }
    }
    return o << "]";
}

Mat operator+(Mat A, Mat B)
{
    Mat R = A;
    for (int i = 0; i < R.data.size(); i++)
        R.data[i] += B.data[i];
    return R;
}

Mat operator-(Mat A, Mat B)
{
    Mat R = A;
    for (int i = 0; i < R.data.size(); i++)
        R.data[i] -= B.data[i];
    return R;
}

Mat operator*(Mat A, Mat B)
{
    if (A.col != B.row) throw std::invalid_argument("invalid sized matrices to multiply");

    Mat R(A.row, B.col, 0);
    for (int i = 0; i < R.row; i++) {
        for (int j = 0; j < R.col; j++) {
            R(i,j) = 0;
            for (int k = 0; k < A.col; k++)
                R(i,j) += A(i,k)*B(k,j);
        }
    }
    return R;
}

Mat operator*(Mat A, float d)
{
    Mat R = A;
    for (int i = 0; i < R.data.size(); i++)
        R.data[i] *= d;
    return R;
}

Mat operator/(Mat A, float d)
{
    if (d == 0) throw runtime_error("division of matrix by zero");

    Mat R = A;
    for (int i = 0; i < R.data.size(); i++)
        R.data[i] /= d;
    return R;
}

float Mat::mag() const
{
    float sum = 0;
    for (float f : data)
        sum += f*f;
    return sqrt(sum);
}

float dot(Mat A, Mat B)
{
    if (A.row == B.row and A.col == B.col) {
        float sum = 0;
        for (int i = 0; i < A.data.size(); i++)
            sum += A(i)*B(i);
    } else throw invalid_argument("invalid sized matrices to dot product");
}
