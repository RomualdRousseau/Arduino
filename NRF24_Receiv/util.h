byte array_get_bit(const byte buf[], const int off) {
  int p = off / 8;
  int b = off % 8;
  return (buf[p] >> b) & 0x01;
}
