/**
 * @description: sequential method to get global maxima
 * @param {int*} array: target array
 * @param {int} size_of_array: the size of the target array
 * @return {int} the global maxima of the target value
 */
int get_maxima_sequential(int* array, int size_of_array) {

    if ( size_of_array <= 0 ) {
        return -1;
    }

    int maxima = array[0];

    for ( int i = 1; i < size_of_array; i++ ) {
        if ( array[i] > maxima ) {
            maxima = array[i];
        }
    }

    return maxima;

}
