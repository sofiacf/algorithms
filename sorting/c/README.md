# Sorting

## Run Times
The actual run times for sorting various size integer arrays using the C
implementation of the algorithms is shown below. All data was collected using a
docker container running on a Surface Book 2 Laptop (Intel Core i7, 16GB RAM).
Each time value represents the median of 3 separate executions.

### Arrays w/ Random Integer Values

![alt text](RANDOM.png "RANDOM ARRAY VALUES") 


|ALGORITHM| n=100 |n=1000 |n=10000 |n=100000 |
--|--|--|--|--|
|C_QSORT |0.000014 sec|0.000168 sec|0.002225 sec|0.025716 sec|
|BUBBLE |0.000062 sec|0.006267 sec|0.629706 sec|36.904889 sec|
|INSERTION |0.000010 sec|0.001021 sec|0.077240 sec|7.530338 sec|
|SELECTION |0.000012 sec|0.000871 sec|0.076891 sec|7.618721 sec|
|QUICK |0.000010 sec|0.000226 sec|0.001607 sec|0.019639 sec|
|MERGE |0.000007 sec|0.000161 sec|0.001128 sec|0.013671 sec|

### Arrays w/ Pre-Sorted Values

![alt text](SORTED.png "SORTED ARRAY VALUES") 

|ALGORITHM| n=100 |n=1000 |n=10000 |n=100000 |
--|--|--|--|--|
|C_QSORT |0.000002 sec|0.000018 sec|0.000451 sec|0.004470 sec|
|BUBBLE |0.000000 sec|0.000002 sec|0.000016 sec|0.000406 sec|
|INSERTION |0.000001 sec|0.000014 sec|0.000256 sec|0.002805 sec|
|SELECTION |0.000010 sec|0.001810 sec|0.074741 sec|7.511977 sec|
|QUICK |0.000008 sec|0.000127 sec|0.001200 sec|0.015383 sec|
|MERGE |0.000004 sec|0.000000 sec|0.000429 sec|0.004537 sec|

### Arrays w/ Reverse Sorted Values

![alt text](REVERSED.png "REVERSED ARRAY VALUES") 

|ALGORITHM| n=100 |n=1000 |n=10000 |n=100000 |
--|--|--|--|--|
|C_QSORT |0.000002 sec|0.000017 sec|0.000246 sec|0.004992 sec|
|BUBBLE |0.000042 sec|0.008379 sec|0.393757 sec|40.858194 sec|
|INSERTION |0.000016 sec|0.001625 sec|0.152922 sec|15.284166 sec|
|SELECTION |0.000010 sec|0.000957 sec|0.084897 sec|8.473309 sec|
|QUICK |0.000007 sec|0.000142 sec|0.001300 sec|0.015819 sec|
|MERGE |0.000004 sec|0.000035 sec|0.000345 sec|0.004488 sec|