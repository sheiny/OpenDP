#include "circuit.h"

using std::cout;
using std::endl;
using std::string;

namespace opendp {

rect::rect()
  : xLL(std::numeric_limits< double >::max()),
    yLL(std::numeric_limits< double >::max()),
    xUR(std::numeric_limits< double >::min()),
    yUR(std::numeric_limits< double >::min()) {};

  
void rect::print() { 
  printf("%f : %f - %f : %f\n", xLL, yLL, xUR, yUR);
  fflush(stdout); 
}

  
site::site() 
  : name(""), width(0.0), height(0.0), type("") {};

site::site(const site& s)
  : name(s.name),
    width(s.width),
    height(s.height),
    type(s.type),
    symmetries(s.symmetries) {};
  
mincut::mincut()
  : via_num(0), width(0.0), length(0.0), 
    within(0.0), direction("") {};
  
space::space() 
  : adj(0), type(""), min(0), max(0) {};

  
layer::layer()
  : name(""),
    type(""),
    direction(""),
    xPitch(0.0),
    yPitch(0.0),
    xOffset(0.0),
    yOffset(0.0),
    width(0.0),
    maxWidth(0.0),
    spacing(""),
    minStep(""),
    area(0.0),
    minEnclosedArea(0.0) {};
  
viaRule::viaRule() : name("") {};

via::via() : name(""), viaRule(""), property("") {};

macro_pin::macro_pin() : direction(""), shape(""), layer(0) {};

macro::macro()
  : name(""),
    type(""),
    isFlop(false),
    isMulti(false),
    xOrig(0.0),
    yOrig(0.0),
    width(0.0),
    height(0.0),
    edgetypeLeft(0),
    edgetypeRight(0) {
#ifdef USE_GOOGLE_HASH
  pins.set_empty_key(INITSTR);
#endif
}

void macro::print() {
  cout << "|=== BEGIN MACRO ===|" << endl;
  cout << "name:                " << name << endl;
  cout << "type:                " << type << endl;
  cout << "(xOrig,yOrig):       " << xOrig << ", " << yOrig << endl;
  cout << "[width,height]:      " << width << ", " << height << endl;
  for(unsigned i = 0; i < sites.size(); ++i) {
    cout << "sites[" << i << "]: " << sites[i] << endl;
  }
  for(OPENDP_HASH_MAP< string, macro_pin >::iterator it = pins.begin();
      it != pins.end(); it++) {
    cout << "pins: " << (*it).first << endl;
  }
  cout << "|=== BEGIN MACRO ===|" << endl;
}


pin::pin() 
  : name(""),
    id(UINT_MAX),
    owner(UINT_MAX),
    net(UINT_MAX),
    type(UINT_MAX),
    isFlopInput(false),
    isFlopCkPort(false),
    x_coord(0.0),
    y_coord(0.0),
    x_offset(0.0),
    y_offset(0.0),
    isFixed(false) {};

void pin::print() {
  cout << "|=== BEGIN PIN ===|  " << endl;
  cout << "name:                " << name << endl;
  cout << "id:                  " << id << endl;
  cout << "type:                " << type << endl;
  cout << "net:                 " << net << endl;
  cout << "pin owner:           " << owner << endl;
  cout << "isFixed?             " << (isFixed ? "yes" : "no") << endl;
  cout << "(x_coord,y_coord):   " << x_coord << ", " << y_coord << endl;
  cout << "(x_offset,y_offset): " << x_offset << ", " << y_offset << endl;
  cout << "|===  END  PIN ===|  " << endl;
}
  
cell::cell()
      : name(""),
        type(UINT_MAX),
        id(UINT_MAX),
        x_coord(0),
        y_coord(0),
        init_x_coord(0),
        init_y_coord(0),
        x_pos(INT_MAX),
        y_pos(INT_MAX),
        width(0.0),
        height(0.0),
        isFixed(false),
        isPlaced(false),
        inGroup(false),
        hold(false),
        region(UINT_MAX),
        cellorient(""),
        group(""),
        dense_factor(0.0),
        dense_factor_count(0),
        binId(UINT_MAX),
        disp(0.0) {
#ifdef USE_GOOGLE_HASH
    ports.set_empty_key(INITSTR);
#endif
}


pixel::pixel()
  : name(""),
    util(0.0),
    x_pos(0.0),
    y_pos(0.0),
    group(UINT_MAX),
    linked_cell(NULL),
    isValid(true) {};

net::net() 
  : name(""), source(UINT_MAX) {};

row::row()
      : name(""),
        site(UINT_MAX),
        origX(0),
        origY(0),
        stepX(0),
        stepY(0),
        numSites(0),
        siteorient("") {};

group::group() : name(""), type(""), tag(""), util(0.0) {};

void group::print(std::string gName) {
  std::cout << gName << " name : " << name << " type : " << type
       << " tag : " << tag << " end line " << std::endl;
  for(int i = 0; i < regions.size(); i++) {
    regions[i].print();
  }
};

sub_region::sub_region()
  : x_pos(0), y_pos(0), width(0), height(0) {
  siblings.reserve(8192);
}


  
void layer::print() {
  cout << "|=== BEGIN LAYER ===|" << endl;
  cout << "name:               " << name << endl;
  cout << "type:               " << type << endl;
  cout << "direction:          " << direction << endl;
  cout << "[xPitch,yPitch]:    " << xPitch << ", " << yPitch << endl;
  cout << "[xOffset,yOffset]:  " << xOffset << ", " << yOffset << endl;
  cout << "width:              " << width << endl;
  cout << "|===  END  LAYER ===|" << endl;
}


circuit::circuit() 
  : GROUP_IGNORE(false),
    num_fixed_nodes(0),
    num_cpu(1),
    DEFVersion(""),
    DEFDelimiter("/"),
    DEFBusCharacters("[]"),
    design_name(""),
    DEFdist2Microns(0),
    sum_displacement(0.0),
    displacement(400.0),
    max_disp_const(0.0),
    max_utilization(100.0),
    wsite(0),
    max_cell_height(1),
    rowHeight(0.0f), 
    fileOut(0) {

  macros.reserve(128);
  layers.reserve(32);
  rows.reserve(4096);
  sub_regions.reserve(100);

#ifdef USE_GOOGLE_HASH
  macro2id.set_empty_key(
      INITSTR); /* OPENDP_HASH_MAP between macro name and ID */
  cell2id.set_empty_key(
      INITSTR); /* OPENDP_HASH_MAP between cell  name and ID */
  pin2id.set_empty_key(
      INITSTR); /* OPENDP_HASH_MAP between pin   name and ID */
  net2id.set_empty_key(
      INITSTR); /* OPENDP_HASH_MAP between net   name and ID */
  row2id.set_empty_key(
      INITSTR); /* OPENDP_HASH_MAP between row   name and ID */
  site2id.set_empty_key(
      INITSTR); /* OPENDP_HASH_MAP between site  name and ID */
  layer2id.set_empty_key(
      INITSTR); /* OPENDP_HASH_MAP between layer name and ID */
  via2id.set_empty_key(INITSTR);
  group2id.set_empty_key(INITSTR); /* group between name -> index */
#endif
};

int IntConvert(double fp) {
  return (int)(fp + 0.5f);
}

}
