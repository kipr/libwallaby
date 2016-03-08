#ifdef SWIGJAVA

%extend Camera::CameraDevice
{
  const Channel *getChannel(int num) const
  {
    return $self->channels()[num];
  }
  
  const int getChannelCount() const
  {
    return $self->channels().size();
  }
}

%extend Camera::Channel
{
  Object getObject(int num) const
  {
    return (*$self->objects())[num];
  }
  
  int getObjectCount() const
  {
    return $self->objects()->size();
  }
}

%template (IntPoint2) Point2<int>;
%template (IntRect) Rect<int>;

%extend Camera::Object
{
  Rect<int> getBoundingBox() const
  {
    const Rect<unsigned> &a = $self->boundingBox();
    return Rect<int>(a.x(), a.y(), a.width(), a.height());
  }
  
  Point2<int> getCentroid() const
  {
    const Point2<unsigned> &a = $self->centroid();
    return Point2<int>(a.x(), a.y());
  }
}

#endif
