%extend Create
{
  %apply (char *STRING, size_t LENGTH) { (const char data[], size_t len) }
  %ignore write(const unsigned char *, const size_t &);
  bool write(const char data[], size_t len)
  {
    return $self->write(reinterpret_cast<const unsigned char *>(data), len);
  }
}
