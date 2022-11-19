#pragma once
/**
 * Base class for solvers of finite element equations
 */
class CFE_EquationSolver
{
public:

	CFE_EquationSolver(void);
	~CFE_EquationSolver(void);
	//! Solve function 
    /*!
	  solves finite element equations
    */
	virtual bool Solve() =0;
};
