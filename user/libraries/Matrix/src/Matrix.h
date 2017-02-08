/*
************************************************************************
* 作者:  IntoRobot Team 
* 版本:  V1.0.0
* 版权： Copyright (c) 2016 Shenzhen MOLMC Technology Co., Ltd..
* 日期:  Dec. 21 2016
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
描述：基本的矩阵操作
*/
namespace mm
{
	template <typename T, unsigned m, unsigned n>
	class Matrix
	{
		public:
		Matrix<T,n,m> Transpose()
		{
			Matrix<T,n,m> transposed;
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < m; j++)
				{
					transposed(i,j) = this->_elements[j][i];
				}
			}
			return transposed;
		}
		Matrix<T,m,n> Invert()   //only for 2*2
		{
			Matrix<T,m,n> inverse;
			float det;
			det = this->_elements[0][0] * this->_elements[1][1] - this->_elements[0][1] * this->_elements[1][0];
			inverse(0,0) =   this->_elements[1][1] / det;
			inverse(0,1) = - this->_elements[0][1] / det;
			inverse(1,0) = - this->_elements[1][0] / det;
			inverse(1,1) =   this->_elements[0][0] / det;
			return inverse;
		}
		T get(const unsigned& row, const unsigned& col)
		{
			return _elements[row][col];
		}
		void set(const unsigned& row, const unsigned& col, T value)
		{
			_elements[row][col] = value;
		}
		T& operator()(const unsigned& row, const unsigned& col)
		{
			return this->_elements[row][col];
		}
		const T& operator()(const unsigned& row, const unsigned& col) const
		{
			return this->_elements[row][col];
		}
		Matrix<T,m,n>& operator+=(const Matrix<T,m,n> &rhs)
		{
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					this->_elements[i,j] += rhs(i,j);
				}
			}
			return *this;
		}
		Matrix<T,m,n>& operator-=(const Matrix<T,m,n> &rhs)
		{
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					this->_elements[i,j] -= rhs(i,j);
				}
			}
			return *this;
		}
		Matrix<T,m,n>& operator*=(const T &rhs)
		{
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					this->_elements[i,j] *= rhs;
				}
			}
			return *this;
		}
		private:
		T _elements[m][n] =
		{
			{
				0
			}
		}
		;
	}
	;
	using Vector2f = Matrix<float, 2, 1>;
	using Vector3f = Matrix<float, 3, 1>;
	using Matrix3f = Matrix<float, 3, 3>;
	using Vector3d = Matrix<double, 3, 1>;
	using Matrix3d = Matrix<double, 3, 3>;
	template <typename T, unsigned m, unsigned n>
	Matrix<T,m,n> operator+(const Matrix<T,m,n> &lhs, const Matrix<T,m,n> &rhs)
	{
		Matrix<T,m,n> result;
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result(i, j) = lhs(i,j) + rhs(i,j);
			}
		}
		return result;
	}
	template <typename T, unsigned m, unsigned n>	
	Matrix<T,m,n> operator-(const Matrix<T,m,n> &lhs, const Matrix<T,m,n> &rhs)
	{
		Matrix<T,m,n> result;
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result(i, j) = lhs(i,j) - rhs(i,j);
			}
		}
		return result;
	}
	template <typename T, unsigned m, unsigned n, unsigned o>
	Matrix<T,m,o> operator*(const Matrix<T,m,n> &lhs, const Matrix<T,n,o> &rhs)
	{
		Matrix<T,m,o> result;
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < o; j++)
			{
				for (int k = 0; k < n; k++)
				{
					result(i, j) += lhs(i,k) * rhs(k,j);
				}
			}
		}
		return result;
	}
	template <typename T, unsigned m, unsigned n>
	Matrix<T,m,n> operator*(const Matrix<T,m,n> &lhs, const T &rhs)
	{
		Matrix<T,m,n> result;
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result(i, j) = lhs(i,j) * rhs;
			}
		}
		return result;
	}
}