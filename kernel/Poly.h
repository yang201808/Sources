//$Id: Poly.h,v 1.5 2005-08-16 12:20:23 bricken Exp $



#ifndef POLYCPP_HEADER
#define POLYCPP_HEADER
#include "mod2.h"

#include "numbers.h"
#include "Number.h"
#include "febase.h"
#include "polys.h"
#include "ring.h"


#include <boost/shared_ptr.hpp>

#include <vector>

#define BOOST_DISABLE_THREADS


//PolyImpl is a 08/15 poly wrapper
//Poly wraps around PolyImpl with reference counting using boost
class PolyImpl{
  friend class Poly;
  //friend class Number;
 protected:
  poly getInternalReference() const{
    return p;
  }
 public:
  ring getRing() const{
    return r;
  }
  friend PolyImpl operator+(const PolyImpl& p1, const PolyImpl& n2);
  friend PolyImpl operator-(const PolyImpl& p1, const PolyImpl& n2);
  friend PolyImpl operator/(const PolyImpl& p1, const PolyImpl& n2);
  friend PolyImpl operator*(const PolyImpl& p1, const PolyImpl& n2);
  friend bool operator==(const PolyImpl& p1, const PolyImpl& n2);
  friend PolyImpl operator+(const PolyImpl& p1, int n2);
  friend PolyImpl operator-(const PolyImpl& p1, int n2);
  friend PolyImpl operator/(const PolyImpl& p1, int n2);
  friend PolyImpl operator*(const PolyImpl& p1, int n2);
  friend bool operator==(const PolyImpl& p1, int n2);
  PolyImpl& operator=(const PolyImpl& p2){
    //durch Reihenfolge Selbstzuweisungen berücksichtigt
    if (this==&p2) return *this;
    poly pc=p_Copy(p2.p,p2.r);
    p_Delete(&p,r);
    r=p2.r;
    p=pc;
    return *this;
  }
  PolyImpl operator-(){
    PolyImpl t(*this);
    t.p=p_Copy(p,r);
    t.p=p_Neg(t.p,r);
    return t;
  }
  PolyImpl& operator+=(const PolyImpl & p2){
    if (r!=p2.r){
      Werror("not the same ring");
      return *this;
    }
    if (this==&p2){
      number two=n_Init(2,r);
      p_Mult_nn(p,two,r);
      return *this;
    }
    p=p_Add_q(p,p_Copy(p2.p,p2.r),r);
   
    return *this;
  }
  PolyImpl& operator*=(const PolyImpl & p2){
    if (r!=p2.r){
      Werror("not the same ring");
      return *this;
    }
    if (this==&p2){
      poly pc=p_Copy(p,r);
      p=p_Mult_q(p,p2.p,r);
      return *this;
    }
    p=p_Mult_q(p,p_Copy(p2.p,p2.r),r);
    return *this;
  }
  PolyImpl& operator*=(const Number & n){
    if (r!=n.r){
      Werror("not the same ring");
      return *this;
    }
    
    p=p_Mult_nn(p,n.n,r);
    return *this;
  }
  PolyImpl& operator-=(const PolyImpl & p2){
    if (r!=p2.r){
      Werror("not the same ring");
      return *this;
    }
    if (this==&p2){
      p_Delete(&p,r);
      p=NULL;
      return *this;
    }

    poly pc=p_Copy(p2.p,p2.r);
    pc=p_Neg(pc,r);
    p=p_Add_q(p,pc,r);

    
    return *this;
  }
  //Div not available for rings other than currRing
/*   PolyImpl& operator/=(const PolyImpl & p2){ */
/*     if (r!=p2.r){ */
/*       Werror("not the same ring"); */
/*       return *this; */
/*     } */
/*     if (this==&p2){ */
/*       poly one=p_ISet(1,r); */
/*       p_Delete(&p,r); */
/*       p=one; */
/*       return *this; */
/*     } */
/*     number nv=n_Div(n,p2.n,r); */
/*     n_Delete(&n,r); */
/*     n=nv; */
/*     return *this; */
/*   } */










  PolyImpl& operator=(int n){
 
    p_Delete(&p,r);
    p=p_ISet(n,r);
    return *this;
 
  }
  

  PolyImpl(){
    r=currRing;
    p=NULL;
  }
  PolyImpl(const PolyImpl & p){
    r=p.r;
    this->p=p_Copy(p.p,r);
  }
  PolyImpl(poly p, ring r){
    this->p=p_Copy(p,r);
    this->r=r;
  }
  PolyImpl(poly p, ring r,int){
    this->p=p;
    this->r=r;
  }
  PolyImpl(int n, ring r){
    this->p=p_ISet(n,r);
    this->r=r;
  }
  explicit PolyImpl(int n){
    r=currRing;
    this->p=p_ISet(n,r);
  }
  void print(){
    p_Write(p,r,r);
  }

  virtual ~PolyImpl(){
    p_Delete(&p,r);
  }

 protected:
  poly p;
  ring r;

};

PolyImpl operator+(const PolyImpl &p1, const PolyImpl& p2){
  PolyImpl erg(p1);
  erg+=p2;
  return erg;
}
PolyImpl operator*(const PolyImpl &p1, const PolyImpl& p2){
  PolyImpl erg(p1);
  erg*=p2;
  return erg;
}
PolyImpl operator-(const PolyImpl &p1, const PolyImpl& p2){
  PolyImpl erg(p1);
  erg-=p2;
  return erg;
}
/*PolyImpl operator/(const PolyImpl &p1, const PolyImpl& p2){
  PolyImpl erg(p1);
  erg/=p2;
  return erg;
  }*/
/*
bool operator==(const PolyImpl &p1, const PolyImpl& p2){
  if(p1.r!=p2.r)
    return false;
  return n_Equal(p1.n,p2.n,p1.r);
  }*/
//Equal Polys not available for oth. rings than currRing


PolyImpl operator+(const PolyImpl &p1, int p2){
  PolyImpl erg(p1);
  erg+=PolyImpl(p2,p1.r);
  return erg;
}
PolyImpl operator*(const PolyImpl &p1, int p2){
  PolyImpl erg(p1);
  erg*=PolyImpl(p2,p1.r);
  return erg;
}
PolyImpl operator-(const PolyImpl &p1, int p2){
  PolyImpl erg(p1);
  erg-=PolyImpl(p2,p1.r);
  return erg;
}
/*PolyImpl operator/(const PolyImpl &p1, int p2){
  PolyImpl erg(p1);
  erg/=PolyImpl(p2,p1.r);
  return erg;
  }*/

/*bool operator==(const PolyImpl &p1, int p2){
  return n_Equal(p1.n,PolyImpl(p2,p1.r).n,p1.r);
  }*/
PolyImpl operator+(int p1, const PolyImpl& p2){
  PolyImpl erg(p2);
  return erg+=PolyImpl(p1,p2.getRing());
}
/*PolyImpl operator-(int p1, const PolyImpl& p2){

  PolyImpl erg(p1,p2.r);
  return erg-=p2;
  }*/
/*PolyImpl operator/(int p1, const PolyImpl& p2){
  PolyImpl erg(p1,p2.r);
  return erg/=p2;
  }*/

PolyImpl operator*(int p1, const PolyImpl& p2){
  PolyImpl erg(p2);
  return erg*=PolyImpl(p1,p2.getRing());
}
/*bool operator==(int p1, const PolyImpl& p2){
  return p2==PolyImpl(p1,p2.r);
  }*/
using namespace boost;


template<class T> class ConstTermReference{
 private:
  ring r;
  poly t;
 public:
  operator T(){
    return T(p_Head(t,r),r);
  }
  ConstTermReference(poly p, ring r){
    this->t=p;
    this->r=r;
  }
  
};
template<class T> class PolyInputIterator:
public std::iterator<std::input_iterator_tag,T,int, shared_ptr<const T>,ConstTermReference<T> >
{

  
 private:
  poly t;
  ring r;
  public:
  bool operator==(const PolyInputIterator& t2){
    return t2.t==t;
  }
  bool operator!=(const PolyInputIterator& t2){
    return t2.t!=t;
  }
  PolyInputIterator(poly p, ring r){
    t=p;
    this->r=r;
  }
  PolyInputIterator(const PolyInputIterator& it){
    t=it.t;
    r=it.r;
  }
  PolyInputIterator& operator++(){
    t=t->next;
    return *this;
  }
  PolyInputIterator operator++(int){
    PolyInputIterator it(*this);
    ++(*this);
    return it;
  }
  const ConstTermReference<T> operator*(){
    return ConstTermReference<T> (t,r);
  }
  shared_ptr<const T> operator->(){
    return shared_ptr<const T>(new T(p_Head(t,r),r,0));
  }

};
class Poly{
 
 public:
  void copy_on_write(){
    if (!ptr.unique()){
      ptr.reset(new PolyImpl(*ptr));
    }
  }
  void print() const {
    ptr->print();
  }
  //* ressource managed by Singular
  char* c_string(){

    return p_String(ptr->p,ptr->r,ptr->r);
  }

  Poly(ring r=currRing):ptr(new PolyImpl((poly) NULL,r)){
  }
  Poly(int n, ring r=currRing):ptr(new PolyImpl(n,r)){
    
  }
  Poly(const char* c, ring r=currRing):ptr(new PolyImpl((poly)NULL,r)){
    //p_Read takes no const so do
    char* cp=(char*) omalloc((strlen(c)+1)*sizeof(char));
    strcpy(cp,c);
    p_Read(cp,ptr->p,r);
    omfree(cp);
  }
  Poly(std::vector<int> v, ring r=currRing):ptr(new PolyImpl((poly) NULL,r)){
    unsigned int i;
    int s=v.size();
    poly p=p_ISet(1,r);
    for(i=0;i<v.size();i++){
      pSetExp(p,i+1,v[i]);
    }
    pSetm(p);
    ptr->p=p;
  }
  Poly& operator+=(Poly p2){
    copy_on_write();
    *ptr += *p2.ptr;
    
    return *this;
  }
  Poly& operator*=(Poly p2){
    copy_on_write();
    *ptr *= *p2.ptr;
    
    return *this;
  }
  Poly& operator*=(Number n){
    copy_on_write();
    *ptr *=n;
    
    return *this;
  }
  /*  void print(){
     StringSetS("");
     write();
     Print(StringAppendS(""));
     }*/
  virtual ~Poly(){}
  Poly(poly p, ring r):ptr(new PolyImpl(p,r)){
  }
  Poly(poly p, ring r,int):ptr(new PolyImpl(p,r,0)){
  }
  /*Poly(Poly& p){
    ptr=p.ptr;
    }*/
  Poly(const Poly&p):ptr(p.ptr){
  }
  PolyInputIterator<Poly> begin(){
    return PolyInputIterator<Poly>(ptr->p,ptr->r);
  }
  PolyInputIterator<Poly> end(){
    return PolyInputIterator<Poly>(NULL, ptr->r);
  }
  ring getRing(){
    return ptr->getRing();
  }
 protected:
  Poly(PolyImpl& impl):ptr(&impl){
   
  }
  poly getInternalReference(){
    return ptr->getInternalReference();
  }
 private:
  shared_ptr<PolyImpl> ptr;
  friend inline Poly operator+(const Poly& p1, const Poly& p2);
  friend inline Poly operator*(const Poly& p1, const Poly& p2);
  
};

inline Poly operator+(const Poly& p1, const Poly& p2){
    PolyImpl* res=new PolyImpl(*p1.ptr);
    *res+=*p2.ptr;
    return(Poly(*res));
}
inline Poly operator*(const Poly& p1, const Poly& p2){
    PolyImpl* res=new PolyImpl(*p1.ptr);
    *res *= *p2.ptr;
    return(Poly(*res));
}



#endif
