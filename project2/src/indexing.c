
/**
 * Return the appropriate index given a row @p `y`, a column @p `x` and the
 * width @p `w0` of the matrix.
 */
int yx_index(int y, int x, int w0) { return y * w0 + x; }
