struct PidGains
{
    short p;
    short i;
    short d;
    short pd;
    short id;
    short dd;
};

%extend Motor
{
  PidGains getPidGains() const
  {
    PidGains ret;
    $self->pidGains(ret.p, ret.i, ret.d, ret.pd, ret.id, ret.dd);
    return ret;
  }
  
  void setPidGains(const PidGains p)
  {
    $self->setPidGains(p.p, p.i, p.d, p.pd, p.id, p.dd);
  }
}
