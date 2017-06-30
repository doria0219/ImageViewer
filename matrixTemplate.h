//
//  matrixTemplate.h
//
//  Created by HelloSangShen on 2017/4/27.
//  Copyright © 2017年 HelloSangShen. All rights reserved.
//

#ifndef MATRIXTEMPLATE_H
#define MATRIXTEMPLATE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <stdexcept>
#include <QImage>

using std::range_error;
using std::vector;
using std::cout;
using std::endl;
using std::ostream;

template<typename T>
class Matrix
{
private:
    // point to the data
    T ** _data;
    // number of row and column
    size_t _nRow, _nCol;
    size_t _startR, _startC;
    // flag to indicate row-first or column-first
    int _t;
    bool checkRange(size_t r, size_t c) const;
    void swap(Matrix& mat);
    
public:
    // construct a r-row and c-column matrix with all init element
    Matrix(size_t r, size_t c, T init);
    // copy constructor
    Matrix(const Matrix<T>& mat);
    Matrix<T>& operator=(Matrix<T> mat);
    ~Matrix();
    // get element at r-row and c-column
    // return reference to make it modifiable
    T& operator()(size_t r, size_t c) const;
    // get a submatrix [rs:re,cs:ce]
    Matrix<T> subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const;
    // output the matrix
    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& rhs);
    size_t getNRow() const;
    size_t getNCol() const;
    void setStartR(const size_t sr);
    void setStartC(const size_t sc);
    std::vector<T> getRow(size_t r) const;
    std::vector<T> getCol(size_t c) const;
    bool setRow(size_t r, const std::vector<T>& d);
    bool setCol(size_t c, const std::vector<T>& d);
    // transpose it in-place
    Matrix<T>& transpose();
    // set matrix by 3 *3, the same as QMatrix.setMatrix()
    void setMatrix(T m11, T m12, T m21, T m22, T dx, T dy);
    // operator +
    Matrix<T> operator+(Matrix<T>& otherMat) const;
    Matrix<T> operator+(T factor) const;

    template<typename U>
    friend Matrix<U> operator+(const U factor, Matrix<U>& mat);

    // operator -
    Matrix<T> operator-(Matrix<T>& otherMat) const;
    Matrix<T> operator-(T factor) const;

    template<typename U>
    friend Matrix<U> operator-(const U factor, Matrix<U> & mat);

    // operator *
    Matrix<T> operator*(Matrix<T>& otherMat) const;
    T mul(vector<T> vec1, vector<T> vec2) const;

    Matrix<T> operator*(T factor) const;

    template<typename U>
    friend Matrix<U> operator*(const U factor, Matrix<U> & mat);

    // operator /
    template<typename U>
    friend Matrix<U> operator/(Matrix<U>& mat, const U factor);

    // translate between QImage & Matrix
    static Matrix<T> fromQImage(const QImage&, char);
    static QImage toQImage(const Matrix<int>&);
    static QImage toQImage(const Matrix<double>&);
    static QImage toQImage(const Matrix<int>&, const Matrix<int>&, const Matrix<int>&);
    static QImage toQImage(const Matrix<double>&, const Matrix<double>&, const Matrix<double>&);
    static Matrix<double> abs4complex(const Matrix<std::complex<double> > &);
    static Matrix<T> logtranslate(const Matrix<T> &, T c);
    static void fftshift(Matrix<T> &);
    static Matrix<int> normalization(const Matrix<double> &);
    static Matrix<T> inverseMatrix(const Matrix<T> &, bool *);
    static void map(const Matrix<double> &, const T, const T, T *, T *);
    static Matrix<T> convolution(const Matrix<T> &, const Matrix<T> &);
    static Matrix<T> multiplication(const Matrix<T> &, const Matrix<T> &);
    static Matrix<std::complex<double> > multiplication(const Matrix<std::complex<double> > &, const Matrix<double> &);
    static Matrix<std::complex<double> > multiplication(const Matrix<double> &, const Matrix<std::complex<double> > &);

    static void test4MatrixTranspose();
};

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c, T init):
_nRow(r), _nCol(c), _t(0)
{
    // new _data
    // new _data[i] within loop

    // std::cout << "constructor 1" << std::endl;

    _t = 1;
    _startR = 0;
    _startC = 0;
    
    _data = new T* [r];
    
    for(int i = 0; i < r; i++){
        _data[i] = new T[c];
    }
    
    for(int i = 0; i < r ; i++){
        for (int j = 0; j < c; j++) {
            _data[i][j] = init;
        }
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mat):
_nRow(mat._nRow), _nCol(mat._nCol), _t(mat._t), _startC(mat._startC), _startR(mat._startR)
{
    // new _data
    // new and assign _data[i] within loop

    // std::cout << "constructor 2" << std::endl;

    _t = 1;

    _data = new T* [_nRow];
    
    for(int i = 0; i < _nRow; i++){
        _data[i] = new T[_nCol];
    }
    
    for(int i = 0; i < _nRow ; i++){
        for (int j = 0; j < _nCol; j++) {
            _data[i][j] = mat._data[i][j];
        }
    }
}

template<typename T>
Matrix<T>::~Matrix()
{
    // delete _data[i] first
    // then delete _data
    
    for (int i = 0; i < _nRow; i++) {
        delete _data[i];
    }
    delete _data;
}

template<typename T>
void Matrix<T>::swap(Matrix<T>& mat) {

    std::swap(_data, mat._data);
    std::swap(_nRow, mat._nRow);
    std::swap(_nCol, mat._nCol);
    std::swap(_startC, mat._startC);
    std::swap(_startR, mat._startR);
    std::swap(_t, mat._t);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> mat)
{
    std::cout << "operator =" << std::endl;

    swap(mat);
    return *this;
}

template<typename T>
void Matrix<T>::setStartC(const size_t sc)
{
    this->_startC = sc;
}

template<typename T>
void Matrix<T>::setStartR(const size_t sr)
{
    this->_startR = sr;
}

template<typename T>
size_t Matrix<T>::getNRow() const
{
    // return the number of row
    if(_t == 1)
        return _nRow;
    return _nCol;
}

template<typename T>
size_t Matrix<T>::getNCol() const
{
    // return the number of column
    if(_t == 0)
        return _nRow;
    return _nCol;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(Matrix<T>& otherMat) const
{
    
    if(otherMat.getNRow() == getNRow() && otherMat.getNCol() == getNCol()){
        Matrix<T> matRes = otherMat;
        
        for(int i = 0; i < getNRow(); i++){
            // This is not the best, I wana a better method
            vector<T> row = getRow(i);
            for(int j = 0; j < getNCol(); j++){
                matRes(i, j) = row[j] + otherMat(i, j);
            }
        }
        return matRes;
    }else{
        throw range_error("operator+ :    Range Error");
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator+(T factor) const
{
    Matrix<T> matRes = *this;
    for(int i = 0; i < getNRow(); i++){
        for(int j = 0; j < getNCol(); j++){
            matRes(i, j) = this->operator()(i, j) + factor;
        }
    }
    return matRes;
}

template<typename U>
Matrix<U> operator+(const U factor, Matrix<U>& mat)
{
    return mat + factor;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(Matrix<T>& otherMat) const{

    if(otherMat.getNRow() == getNRow() && otherMat.getNCol() == getNCol()){
        Matrix<T> matRes = otherMat;
        
        for(int i = 0; i < getNRow(); i++){
            // This is not the best, I wana a better method
            vector<T> row = getRow(i);
            for(int j = 0; j < getNCol(); j++){
                matRes(i, j) = row[j] - otherMat(i, j);
            }
        }
        return matRes;
    }else{
        throw range_error("operator+ :    Range Error");
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator-(T factor) const
{
    Matrix<T> matRes = *this;
    for(int i = 0; i < getNRow(); i++){
        for(int j = 0; j < getNCol(); j++){
            matRes(i, j) = this->operator()(i, j) - factor;
        }
    }
    return matRes;
}

template<typename U>
Matrix<U> operator-(const U factor, Matrix<U>& mat)
{
    Matrix<U> matRes = mat;
    for(int i = 0; i < mat.getNRow(); i++){
        for(int j = 0; j < mat.getNCol(); j++){
            matRes(i, j) = factor - mat(i, j);
        }
    }
    return matRes;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(Matrix<T>& otherMat) const{
    
    if(otherMat.getNRow() == getNCol()){
        // It is terrible, ugly
        Matrix<T> matRes = Matrix(getNRow(), otherMat.getNCol(), otherMat(0,0));
        
        for(int i = 0; i < getNRow(); i++){
            for(int j = 0; j < otherMat.getNCol(); j++){
                matRes(i, j) = mul(getRow(i), otherMat.getCol(j));
            }
        }
        return matRes;
    }else{
        throw range_error("operator+ :    Range Error");
    }
}

template<typename T>
T Matrix<T>::mul(vector<T> vec1, vector<T> vec2) const{
    
    if(vec1.size() != vec2.size()){
        throw range_error("operator* :    Size Error");
    }
    else{
        // ...
        T t = vec1[0] * vec2[0];
        for(int i = 1; i < vec1.size(); i++)
            t += vec1[i] * vec2[i];
        return t;
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator*(T factor) const
{
    Matrix<T> matRes = *this;
    for(int i = 0; i < getNRow(); i++){
        for(int j = 0; j < getNCol(); j++){
            matRes(i, j) = this->operator()(i, j) * factor;
        }
    }
    return matRes;
}

template<typename U>
Matrix<U> operator*(const U factor, Matrix<U>& mat)
{
    return mat * factor;
}

template<typename U>
Matrix<U> operator/(Matrix<U>& mat, const U factor)
{
    Matrix<U> matRes = mat;
    for(int i = 0; i < mat.getNRow(); i++){
        for(int j = 0; j < mat.getNCol(); j++){
            matRes(i, j) = mat(i, j)/factor;
        }
    }
    return matRes;
}


template<typename T>
bool Matrix<T>::checkRange(size_t r, size_t c) const
{
    // check if r-row and c-column is out of range
    if(_t == 1){
        if(r >= _nRow || c >= _nCol)
            return false;
    }else if(_t == 0){
        if(r >= _nCol || c >= _nRow)
            return false;
    }
    
    return true;
}

template<typename T>
T& Matrix<T>::operator()(size_t r, size_t c) const
{
    // get element in (r,c)
    // remember check first
    // different of _t has different order
    if(!checkRange(r, c)){
        // out of range
        throw range_error("operator() :    Out Of Range");
    }
    
    if(_t == 1){
        return _data[(r+_startR)%_nRow][(c+_startC)%_nCol];
    }else{
        return _data[(c+_startC)%_nCol][(r+_startR)%_nRow];
    }
}

template<typename T>
Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
{
    // note the validity of value:rs, re, cs, ce
    // try to use "except" to broadcast the exception
    
    if(_t == 1){
        if(!(rs <= re && re < _nRow && cs <= ce && ce < _nCol)){
            // out of range
            throw range_error("subMatrix :    Parameters Out Of Range");
        }else{
            Matrix<T> newMat =  Matrix<T>(re - rs + 1, ce - cs + 1, 0);
            for(int i = 0; i < newMat._nRow; i++){
                for(int j = 0; j < newMat._nCol; j++){
                    newMat._data[i][j] = _data[i + rs][j + cs];
                }
            }
            return newMat;
        }
    }else {
        if(!(rs <= re && re <= _nCol && cs <= ce && ce <= _nRow)){
            // out of range
            throw range_error("subMatrix :    Parameters Out Of Range");
        }else{
            Matrix<T> newMat =  Matrix<T>(ce - cs + 1, re - rs + 1, 0);
            for(int i = 0; i < newMat._nRow; i++){
                for(int j = 0; j < newMat._nCol; j++){
                    // ...
                    newMat._data[i][j] = _data[i + cs][j + rs];
                }
            }
            return newMat;
        }
    }
    
    //...
    return Matrix<int>(1,1,0);
}

template<typename T>
std::vector<T> Matrix<T>::getRow(size_t r) const
{
    // get all element in r-row and push it back in a vector
    // remember check range first
    if((_t == 1 && r > _nRow) || (_t == 0 && r > _nCol)){
        throw range_error("getRow() :    Out Of Range");
    }
    vector<T> vec;
    if(_t == 1){
        for(int i = 0; i < _nCol; i++){
            vec.push_back(_data[r][i]);
        }
    }else{
        for(int i = 0; i < _nRow; i++){
            vec.push_back(_data[i][r]);
        }
    }
    return vec;
}

template<typename T>
std::vector<T> Matrix<T>::getCol(size_t c) const
{
    // get all element in c-column and push it back in a vector
    // remember check range first
    if((_t == 1 && c > _nCol) || (_t == 0 && c > _nRow)){
        throw range_error("getCol() :    Out Of Range");
    }
    vector<T> vec;
    if(_t == 1){
        for(int i = 0; i < _nRow; i++){
            vec.push_back(_data[i][c]);
        }
    }else{
        for(int i = 0; i < _nCol; i++){
            vec.push_back(_data[c][i]);
        }
    }
    return vec;
}

template<typename T>
bool Matrix<T>::setRow(size_t r, const vector<T>& d)
{
    // set the element of the r-row
    // remember check range first
    
    if(_t == 1 && checkRange(r, d.size() - 1)){
        if(d.size() == _nCol){
            for(int i = 0; i < d.size(); i++){
                _data[r][i] = d[i];
            }
            return true;
        }else{
            throw range_error("setRow :    illegal size of vector");
        }
    }else if(_t == 0 && checkRange(d.size() - 1, r)){
        if(d.size() == _nRow){
            for(int i = 0; i < d.size(); i++){
                _data[i][r] = d[i];
            }
            return true;
        }else{
            throw range_error("setRow :    illegal size of vector");}
    }
    return false;
}

template<typename T>
bool Matrix<T>::setCol(size_t c, const vector<T>& d)
{
    // set the element of the c-column
    // remember check range first
    
    if(_t == 1 && checkRange(d.size() - 1, c)){
        if(d.size() == _nRow){
            for(int i = 0; i < d.size(); i++){
                _data[i][c] = d.at(i);
            }
            return true;
        }else{
            throw range_error("setRow :    illegal size of vector");
        }
    }else if(_t == 0 && checkRange(c, d.size() - 1)){
        if (d.size() == _nCol) {
            for(int i = 0; i < d.size(); i++){
                _data[c][i] = d.at(i);
            }
            return true;
        }else{
            throw range_error("setRow :    illegal size of vector");
        }
    }
    return false;
}

template<typename T>
Matrix<T>& Matrix<T>::transpose()
{
    // change _t
    // swap _nRow and _nCol
    // the fastest transpose in the universe ?
    if(_t == 0){
        _t = 1;
    }else if( _t == 1){
        _t = 0;
    }
    return *this;
}

template<typename T>
void Matrix<T>::setMatrix(T m11, T m12, T m21, T m22, T dx, T dy)
{
    if(this->getNCol() != 3 || this->getNRow() != 3){
        return;
    }

    this->operator ()(0, 0) = m11;
    this->operator ()(0, 1) = m12;
    this->operator ()(1, 0) = m21;
    this->operator ()(1, 1) = m22;
    this->operator ()(2, 0) = dx;
    this->operator ()(2, 1) = dy;
}

template<typename U>
ostream& operator<<(ostream& out, const Matrix<U>& rhs)
{
    for(size_t i = 0; i < rhs.getNRow(); ++i) {
        for(size_t j = 0; j < rhs.getNCol(); ++j) {
            out << rhs(i,j) << "\t";
        }
        out << endl;
    }
    return out;
}


// translate between QImage & Matrix
template<typename T>
Matrix<T> Matrix<T>::fromQImage(const QImage& img, char patten)
{
    Matrix<T> ret(img.height(), img.width(), 0);

    for(int i = 0; i < img.height(); i++){
        for(int j = 0; j < img.width(); j++){
            if(patten == 'r'){
                ret(i, j) = qRed(img.pixel(j, i));
            }else if(patten == 'g'){
                ret(i, j) = qGreen(img.pixel(j, i));
            }else if(patten == 'b'){
                ret(i, j) = qBlue(img.pixel(j, i));
            }else if(patten == 'h'){
                ret(i, j) = qGray(img.pixel(j, i));
            }
        }
    }

    return ret;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<int>& gray)
{
    int width = gray.getNCol();
    int height = gray.getNRow();
//    std::cout << width << std::endl;
//    std::cout << height << std::endl;

    QImage img = QImage(width, height, QImage::Format_RGB32);
    for(int i = 0; i < width; i++){
        for(int j =0; j < height; j++){
            int g = gray(j, i);
            img.setPixel(i, j, qRgb(g, g, g));
        }
    }
    return img;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<double> & gray)
{
    return Matrix<int>::toQImage(Matrix<int>::normalization(gray));
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<int>& red, const Matrix<int>& green, const Matrix<int>& blue)
{
    // illegal size
    if(red.getNCol() != blue.getNCol() || red.getNCol() != green.getNCol() || green.getNCol() != blue.getNCol() ||
            red.getNRow() != blue.getNRow() || red.getNRow() != green.getNRow() || green.getNRow() != blue.getNRow()){
        throw range_error("toQImage :    illegal size of matrix");
    }

    int width = red.getNCol();
    int height = red.getNRow();
    QImage img = QImage(width, height, QImage::Format_RGB32);
    for(int i = 0; i < width; i++){
        for(int j =0; j < height; j++){
            int r = red(j, i);
            int g = green(j, i);
            int b = blue(j, i);
            img.setPixel(i, j, qRgb(r, g, b));
        }
    }
    return img;
}

template<typename T>
QImage Matrix<T>::toQImage(const Matrix<double>& red, const Matrix<double>& green, const Matrix<double>& blue){
    return Matrix<int>::toQImage(Matrix<int>::normalization(red), Matrix<int>::normalization(green), Matrix<int>::normalization(blue));
}

template<typename T>
Matrix<double> Matrix<T>::abs4complex(const Matrix<std::complex<double> > & mat)
{
    int width = mat.getNCol();
    int height = mat.getNRow();

    Matrix<double> res(height, width, 0);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            res(i, j) = abs(mat(i, j));
        }
    }

    return res;
}

template<typename T>
Matrix<T> Matrix<T>::logtranslate(const Matrix<T> &mat, T c)
{
    int width = mat.getNCol();
    int height = mat.getNRow();

    Matrix<T> res(height, width, 0);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            res(i, j) = c * log(1 + mat(i, j));
        }
    }

    return res;
}

template<typename T>
void Matrix<T>::fftshift(Matrix<T> & mat)
{
    mat.setStartC(mat.getNCol()/2);
    mat.setStartR(mat.getNRow()/2);
}

template<typename T>
Matrix<int> Matrix<T>::normalization(const Matrix<double> &mat)
{
    int width = mat.getNCol();
    int height = mat.getNRow();

    Matrix<int> res(height, width, 0);

    double max = mat(0, 0), min = mat(0, 0);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(mat(i, j) > max){
                max = mat(i, j);
            }
            if(mat(i, j) < min){
                min = mat(i, j);
            }
        }
    }

    double dis = max - min;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            res(i, j) = (int) ((mat(i, j) - min)/dis * 255);
        }
    }

    return res;
}

template<typename T>
Matrix<T> Matrix<T>::multiplication(const Matrix<T> &mat1, const Matrix<T> &mat2)
{
    if((mat1.getNCol() == mat2.getNCol()) && (mat1.getNRow() == mat2.getNRow())){
        Matrix<T> matRes(mat1.getNRow(), mat1.getNCol(), 0);
        for(int i = 0; i < mat1.getNRow(); i++){
            for(int j = 0; j < mat1.getNCol(); j++){
                matRes(i, j) = mat1(i, j) * mat2(i, j);
            }
        }

        return matRes;
    }else{
        throw range_error("multiplication :    illegal size of matrix");
    }
}

template<typename T>
Matrix<std::complex<double> > Matrix<T>::multiplication(const Matrix<std::complex<double> > & matCom, const Matrix<double> & matDou)
{
    if(matCom.getNCol() != matDou.getNCol() || matCom.getNRow() != matDou.getNRow()){
        throw range_error("mulComplexByDouble :    illigal matrix size");
    }

    Matrix<std::complex<double> > retMat(matDou.getNRow(), matDou.getNCol(), 0);
    for(int i = 0; i < matDou.getNRow(); i++){
        for(int j = 0; j < matDou.getNCol(); j++){
            retMat(i, j) = matCom(i, j) * matDou(i, j);
        }
    }

    return retMat;
}

template<typename T>
Matrix<std::complex<double> > Matrix<T>::multiplication(const Matrix<double> & matDou, const Matrix<std::complex<double> > & matCom)
{

    return multiplication(matCom, matDou);
}

template<typename T>
Matrix<T> Matrix<T>::inverseMatrix(const Matrix<T> & mat, bool *isInverse)
{

    // this function just used for matrix with a size of 3*3
    if(mat.getNCol() != 3 || mat.getNRow() != 3){
        *isInverse = false;
        return Matrix(mat);
    }

    T determinant = mat(0, 0)*mat(1, 1)*mat(2, 2) + mat(0, 1)*mat(1, 2)*mat(2, 0) + mat(0, 2)*mat(1, 0)*mat(2, 1) -
            mat(0, 2)*mat(1, 1)*mat(2, 0) - mat(0, 1)*mat(1, 0)*mat(2, 2) - mat(0, 0)*mat(1, 2)*mat(2, 1);
    if(determinant == 0){
        *isInverse = false;
        return Matrix(mat);
    }

    *isInverse = true;
    Matrix<T> inversedMat(3, 3, 0);

    inversedMat(0, 0) = (mat(1, 1)*mat(2, 2) - mat(1, 2)*mat(2, 0))/determinant;
    inversedMat(1, 0) = -1 * (mat(1, 0)*mat(2, 2) - mat(1, 2)*mat(2, 0))/determinant;
    inversedMat(2, 0) = (mat(1, 0)*mat(2, 1) - mat(1, 1)*mat(2, 0))/determinant;
    inversedMat(0, 1) = -1 * (mat(0, 1)*mat(2, 2) - mat(0, 2)*mat(2, 1))/determinant;
    inversedMat(1, 1) = (mat(0, 0)*mat(2, 2) - mat(0, 2)*mat(2, 0))/determinant;
    inversedMat(2, 1) = -1 * (mat(0, 0)*mat(2, 1) - mat(0, 1)*mat(2, 0))/determinant;
    inversedMat(0, 2) = (mat(0, 1)*mat(1, 2) - mat(0, 2)*mat(1, 1))/determinant;
    inversedMat(1, 2) = -1 * (mat(0, 0)*mat(1, 2) - mat(0, 2)*mat(1, 0))/determinant;
    inversedMat(2, 2) = (mat(0, 0)*mat(1, 1) - mat(0, 1)*mat(1, 0))/determinant;

    return inversedMat;
}

template<typename T>
void Matrix<T>::map(const Matrix<double> & mat, const T x, const T y, T *mx, T *my)
{
    // map for geometry rotation transform
    // map for matrix by 3*3

    // mat: 「 mat(0, 0)  mat(0, 1)  0 |
    //      | mat(1, 0)  mat(1, 1)  0 |
    //     |     0          0      1 」

    *mx = mat(0, 0)*x + mat(1, 0)*y + mat(2, 0);
    *my = mat(0, 1)*x + mat(1, 1)*y + mat(2, 1);
}

template<typename T>
Matrix<T> Matrix<T>::convolution(const Matrix<T> & imageMat, const Matrix<T> & filterMat)
{
    // input: padded image & unmirrored filter
    Matrix<T> fliter = Matrix(filterMat);
    for(int i = 0; i < fliter.getNRow(); i++){
        for(int j = 0; j < fliter.getNCol(); j++){
            fliter(i, j) = filterMat(filterMat.getNRow()-i-1, filterMat.getNCol()-j-1);
            fliter(fliter.getNRow()-i-1, fliter.getNCol()-j-1) = filterMat(i, j);
        }
    }
    // cout << fliter << endl;
    Matrix<T> res(imageMat.getNRow() - fliter.getNRow() + 1, imageMat.getNCol() - fliter.getNCol() + 1, 0);
    for(int i = 0; i < res.getNRow(); i++){
        for(int j = 0; j < res.getNCol(); j++){
            T temp = 0;
            for(int x = 0; x < fliter.getNRow(); x++){
                for(int y = 0; y < fliter.getNCol(); y++){
                    temp += fliter(x, y) * imageMat(y+i, x+j);
                }
            }
            res(i, j) = temp;
        }
    }
    return res;
}



template<typename T>
void Matrix<T>::test4MatrixTranspose()
{
    std::cout << "--------------------initialize matrix a--------------------" << std::endl;
    Matrix<int> a(3, 5, 5);
    std::cout << "a._t:    " << a._t << std::endl << a << endl;

    std::cout << "--------------------a.transpose() ==> b--------------------" << std::endl;
    Matrix<int> b = a.transpose();
    std::cout << "a._t:    " << a._t << std::endl << a << endl;
    std::cout << "b._t:    " << b._t << std::endl << b << endl;

    std::cout << "--------------------b.transpose() ==> c--------------------" << std::endl;
    Matrix<int> c = b.transpose();
    std::cout << "b._t:    " << b._t << std::endl << b << endl;
    std::cout << "c._t:    " << c._t << std::endl << c << endl;
}

#endif /* matrixTemplate_h */
