#ifndef BOOST_EXTENSION_BOAT_HPP
#define BOOST_EXTENSION_BOAT_HPP
//  See the FAQ for info about why the following is necessary
//  here, but usually isn't.
#define BOOST_EXTENSION_VEHICLE_DECL BOOST_EXTENSION_IMPORT_DECL
#include "vehicle.hpp"
#include <iostream>
#include <typeinfo>

class BOOST_EXTENSION_BOAT_DECL boat : virtual public vehicle
{
public:
  boat(void){std::cout << "\nCreated a Boat";}
  virtual ~boat(void){std::cout << "\nDestroyed a Boat";}
  virtual std::string list_capabilities(void);
};

#endif