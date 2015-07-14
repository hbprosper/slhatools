# slhatools
Some simple SLHA tools. This package depends on **ROOT** from [CERN](https://root.cern.ch/drupal).

## Setup
```
  cd slhatools
  source setup.sh (or setup.csh)
  make
```

## Test
This example reads the SLHA file pMSSM.slha and prints out the branching ratios of the heavy neutral scalar Higgs boson.
```
  cd examples
  ./example1.py
```
This is the printout:
```
BR(H -> b bb)           	=  4.987e-01
BR(H -> tau+ tau-)      	=  1.152e-01
BR(H -> mu+ mu-)        	=  4.073e-04
BR(H -> s sb)           	=  4.971e-04
BR(H -> c cb)           	=  3.751e-07
BR(H -> t tb)           	=  3.876e-02
BR(H -> g g)            	=  1.458e-05
BR(H -> gam gam)        	=  1.623e-08
BR(H -> Z gam)          	=  1.755e-08
BR(H -> W+ W-)          	=  3.424e-05
BR(H -> Z Z)            	=  1.710e-05
BR(H -> h h)            	=  1.149e-04
BR(H -> A A)            	=  2.239e-23
BR(H -> Z A)            	=  3.557e-17
BR(H -> ~chi_10 ~chi_10)	=  2.406e-04
BR(H -> ~chi_10 ~chi_20)	=  1.897e-02
BR(H -> ~chi_10 ~chi_30)	=  3.572e-02
BR(H -> ~t_1 ~t_1*)     	=  6.138e-02
BR(H -> ~tau_1- ~tau_1+)	=  1.684e-03
BR(H -> ~tau_2- ~tau_2+)	=  7.738e-04
BR(H -> ~tau_1- ~tau_2+)	=  1.137e-01
BR(H -> ~tau_2- ~tau_1+)	=  1.137e-01
```
