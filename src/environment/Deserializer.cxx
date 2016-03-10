//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: Deserializer.cxx,v 1.12 2007/01/01 18:04:47 stephena Exp $
//============================================================================

#include "Deserializer.hxx"
#include <sstream>
#include <cstring>
using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Deserializer::Deserializer(const string stream_str):
myStream(stream_str) {
    
}




// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Deserializer::close(void)
{
  myStream.clear();
}



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Deserializer::getInt(void)
{
  if(myStream.eof())
    throw "Deserializer: end of file";

  int val = 0;
  unsigned char buf[4];
  myStream.read((char*)buf, 4);
  for(int i = 0; i < 4; ++i)
    val += (int)(buf[i]) << (i<<3);

  return val;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Deserializer::getString(void)
{
  int len = getInt();
  string str;
  str.resize((string::size_type)len);
  myStream.read(&str[0], (streamsize)len);

  if(myStream.bad())
    throw "Deserializer: file read failed";

  return str;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Deserializer::getBool(void)
{
  bool result = false;

  int b = getInt();
  if(b == (int)TruePattern)
    result = true;
  else if(b == (int)FalsePattern)
    result = false;
  else
    throw "Deserializer: data corruption";

  return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Deserializer::getIntArray(int* array, size_t& size)
{
//  size = getInt();
//  array = new char[size];
//  myStream.read(&array[0], (streamsize)size);
//  size = getInt();
//	myStream.read(array, size);

//	cout << myStream << endl;
//	myStream.read((char*)array, size);

//	myStream.read(array, size);

	for(int i = 0 ; i < size ; i++){
		array[i] = getInt();
	}

  if(myStream.bad())
    throw "Deserializer: file read failed";
}
