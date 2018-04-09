// create unsigned char matrix
#define NR_END  1

unsigned char **cmatrix(long row1, long row2, long col1, long col2)
{
  int i, nrow=row2-row1+1, ncol=col2-col1+1;
  unsigned char **c;
  c = new unsigned char* [(nrow+NR_END)*sizeof(unsigned char*)];

  c[row1] = new unsigned char [(ncol*nrow+NR_END)*sizeof(unsigned char)];
  for(i=row1+1; i<=row2; i++)  c[i] = c[i-1] + ncol;

  return c;
}

void free_cmatrix(unsigned char **m, long row1, long row2, long col1, long col2)
{
  delete [] (m[row1]+col1-NR_END);
  delete [] (m+col1-NR_END);
}  

