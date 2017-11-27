--
-- Создано в MAGMA Версия 1.4.145 (Ревизия: 111720) 23.11.2017
-- Type: Gidrodinamika
--
-- DIMENS
-- 100 100 1 /
--

-- STEP 0
DATES
1 'APR' 2016 /
/
 
WELSPECS
'inj1' 'inlet' 		3 	98 1* 'WATER' /
'inj2' 'inlet' 		98 	3  1* 'WATER' /
'prod1' 'outlet'	3 	3  1* 'OIL' /
'prod2' 'outlet' 	98	98 1* 'OIL' /
 /
  
COMPDAT
'inj1' 	3 	98 	1 1 'OPEN' 2* 0.146 /
'inj2' 	98 	3 	1 1 'OPEN' 2* 0.146 /
'prod1' 3 	3 	1 1 'OPEN' 2* 0.146 /
'prod2' 98	98 	1 1 'OPEN' 2* 0.146 /
 /
  
WCONPROD
'prod1' 'OPEN' 'BHP' 5* 100 /
'prod2' 'OPEN' 'BHP' 5* 100 /
 /
  
WCONINJE
'inj1' 'WATER' 'OPEN' 'RATE' 10000 /
'inj2' 'WATER' 'OPEN' 'RATE' 10000 /
 /
  
-- STEP 1
DATES
2 'APR' 2016 /
/
 
TSTEP
500*1
/ 
 
