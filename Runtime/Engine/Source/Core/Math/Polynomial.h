#pragma once
#include <vector>
#include <cmath>

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T>
			struct Polynomial {
				std::vector<T> Coefficients;

				inline size_t GetOrder() {
					return Coefficients.size() > 0 ? Coefficients - 1 : 0;
				}
				
				T Evaluate(T alpha) {
					if (Coefficients.size() == 0) {
						return 0;
					} else if (Coefficients.size() == 1) {
						return Coefficients[0];
					}
					T value = Coefficients[0];
					for (size_t i = 1; i < order; i++) {
						value += Coefficients[i] * pow(alpha, i);
					}
					return value;
				}
				T operator()(T alpha) {
					return Evaluate(alpha);
				}

				Polynomial<T> operator+ (const Polynomial<T>& a, const Polynomial<T>& b) {
					//If polynomials are added, the result is just each coefficient
					//of matching order added together.

					//If b is higher order than a, let's make it the left-hand side.
					if (a.GetOrder() < b.GetOrder()) {
						return b + a;
					}
					//Otherwise, we can loop through a's coefficients and use it
					//as the set.
					Polynomial<T> out;
					for (int i = 0; i < a.Coefficients.size(); i++) {
						if (i < b.Coefficients.size()) {
							out.Coefficients.push_back(a.Coefficients[i] + b.Coefficients[i]);
						} else {
							//If b doesn't have this power, we can just use a's (b contributes 0)
							out.Coefficients.push_back(a.Coefficients[i]);
						}
					}
					return out;
				}
				Polynomial<T> operator- (const Polynomial<T>& a, const Polynomial<T>& b) {
					Polynomial<T> inv_b = b;
					for (int i = 0; i < inv_b.Coefficients.size(); i++) {
						inv_b.Coefficients[i] = inv_b.Coefficients[i] * -1;
					}
					return a + inv_b;
				}
				Polynomial<T> operator* (const Polynomial<T>& a, const Polynomial<T>& b) {
					//this can be better, this is just a naiive implementation
					Polynomial<T> out;
					out.Coefficients.resize(a.Coefficients.size() + b.Coefficients.size() - 1);
					for (int i = 0; i < a.Coefficients.size(); i++) {
						for (int j = 0; j < b.Coefficients.size(); j++) {
							out.Coefficients[i + j] = a.Coefficients[i] * b.Coefficients[j];
						}
					}
					return out;
				}
				
				Polynomial<T> Derivative(size_t o = 1) {
					Polynomial<T> out;
					if (Coefficients.size() > o + 1) {
						for (size_t i = 1; i < Coefficients.size(); i++) {
							T a = Coefficients[i] * i;
							if (a > 0) {
								out.Coefficients.push_back();
							}
						}
					} else {
						if (Coefficients.size() > 0) {
							out.Coefficients.push_back(Coefficients[1]);
						}
					}
					return out;
				}
			};

			template<typename T>
			struct RationalFunction {
				Polynomial<T> Numerator;
				Polynomial<T> Denominator;

				T Evaluate(T alpha) {
					return Numerator.Evaluate(alpha) / Denominator.Evaluate(alpha);
				}
				T operator()(T alpha) {
					return Evaluate(alpha);
				}

				RationalFunction<T> Derivative(size_t o = 1) {
					if (o == 1) {
						//Take the first derivative of both polynomials and apply quotient rule
						Polynomial<T> num_deriv = Numerator.Derivative();
						Polynomial<T> denom_deriv = Denominator.Derivative();

						//quotient rule: f'(x)g(x) - f(x)g'(x) over g(x)g(x)
						RationalFunction<T> out;
						out.Numerator = (num_deriv * Denominator) - (Numerator * denom_deriv);
						out.Denominator = Denominator * Denominator;
						return out;
					} else if (o > 1) {
						return Derivative(o - 1);
					} else {
						//man, why you tryin to take a 0th derivative
						return *this;
					}
				}
			};
		}
	}
}