/*
 * This is a helper lib to implement Vectors (dynamic Arrays) in C
 */

/*
 * Constant used for starting capacity of Vectors
 * Value is arbitrary and can be tuned
 */
#define VECTOR_INITIAL_CAPACITY 1

// Define a vector type
typedef struct {
  int size;      // slots used so far
  int capacity;  // total available slots
  int *data;     // array of integers we're storing
} Vector;

/*
 * Function that initializes a vector struct.
 * It sets size to 0,
 * capacity to VECTOR_INITIAL_CAPACITY
 * and allocates an appropriate amount of memory for the underlying data array.
 */
void vector_init(Vector *vector);

/*
 * Appends the given value to the vector.
 * If the underlying data array is full, then calling this function
 * should cause vector->data to expand to accept this value.
 * Increments vector->size.
 */
void vector_append(Vector *vector, int value);

/*
 * Returns a value out of a vector at the given index.
 * If the index is below 0 or greater than vector->size - 1
 * this function should complain about the index being out of bounds.
 */
int vector_get(Vector *vector, int index);

/*
 * Sets the value at the given index to the given value.
 * If the index is greater than the vector->size,
 * this function should expand the vector until it is big enough to contain
 * the index and set the value at that index.
 * It should zero-fill all values in between.
 * vector->size should be incremented accordingly.
 */
void vector_set(Vector *vector, int index, int value);

/*
 * Doubles the underlying data array capacity
 * if vector->size >= vector->capacity
 */
void vector_double_capacity_if_full(Vector *vector);

/*
 * Frees the memory allocated for the data array.
 * We leave freeing of the Vector struct itself to client code
 * (so they can use any sort of pointer they like,
 * be it stack or heap, and then clean up after themselves).
 */
void vector_free(Vector *vector);
