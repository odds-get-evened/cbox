# The Matrix

a 2-dimensional vector where the 2nd dimension are 3D coordinate vectors (i.e., `x, y, z`).

```c++
using MatrixVec = vector<vector<double>>
```

there is a method that can generate a random matrix or using machine learning train an existing set, to produce a new set, as well as a new model.

```c++
MatrixVec gen_rand_matrix(size_t len);
```

other applications can be applied. since the number range is -1.0 to 1.0, the user can normalize data to this range, and plot any data in a siganture 3D object, thus producing a 3D data signature.