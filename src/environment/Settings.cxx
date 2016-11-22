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
// $Id: Settings.cxx,v 1.125 2007/08/22 13:55:40 stephena Exp $
//============================================================================

#include <cassert>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;

//#include "OSystem.hxx"
#include "bspf.hxx"
#include "Settings.hxx"
#include "RleSystem.hxx"

using namespace rle;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::Settings(RleSystem* rlesystem) : myRleSystem(rlesystem) {
    // Add this settings object to the OSystem
    myRleSystem->attach(this);
    setInternal("sound", "false");
    setDefaultSettings();

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::~Settings() {
	myInternalSettings.clear();
	myExternalSettings.clear();
}

void Settings::loadConfig(const char* config_file) {
	string line, key, value;
	string::size_type equalPos, garbage;

	ifstream in(config_file);
	if (!in || !in.is_open()) {
		rle::Logger::Warning << "Warning: couldn't load settings file: "
				<< config_file << std::endl;
		return;
	}

	while (getline(in, line)) {
		// Strip all whitespace and tabs from the line
		while ((garbage = line.find("\t")) != string::npos)
			line.erase(garbage, 1);

		// Ignore commented and empty lines
		if ((line.length() == 0) || (line[0] == ';'))
			continue;

		// Search for the equal sign and discard the line if its not found
		if ((equalPos = line.find("=")) == string::npos)
			continue;

		// Split the line into key/value pairs and trim any whitespace
		key = line.substr(0, equalPos);
		value = line.substr(equalPos + 1, line.length() - key.length() - 1);
		key = trim(key);
		value = trim(value);

		// Check for absent key or value
		if ((key.length() == 0) || (value.length() == 0))
			continue;

		// Only settings which have been previously set are valid
		//ALE  if(int idx = getInternalPos(key) != -1)
		//ALE  setInternal(key, value, idx, true);
		setInternal(key, value);
	}

	in.close();
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::loadConfig() {
	loadConfig(myRleSystem->configFile().c_str());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Settings::loadCommandLine(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		// strip off the '-' character
		string key = argv[i];
		if (key[0] == '-') {
			key = key.substr(1, key.length());

			// Take care of the arguments which are meant to be executed immediately
			// (and then Stella should exit)
			if (key == "help" || key == "listrominfo") {
				usage();
				setExternal(key, "true");
				return "";
			}

			// Take care of arguments without an option
			if (key == "rominfo" || key == "debug" || key == "holdreset"
					|| key == "holdselect" || key == "holdbutton0") {
				setExternal(key, "true");
				continue;
			}

			if (++i >= argc) {
				rle::Logger::Error << "Missing argument for '" << key << "'"
						<< endl;
				return "";
			}
			string value = argv[i];

			// Settings read from the commandline must not be saved to
			// the rc-file, unless they were previously set
			if (int idx = getInternalPos(key) != -1)
				setInternal(key, value, idx);   // don't set initialValue here
			else
				setExternal(key, value);
		} else
			return key;
	}

	return "";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::validate() {
	string s;
	int i;

	s = getString("video");
	if (s != "soft" && s != "gl")
		setInternal("video", "soft");

#ifdef DISPLAY_OPENGL
	s = getString("gl_filter");
	if(s != "linear" && s != "nearest")
	setInternal("gl_filter", "nearest");

	i = getInt("gl_aspect");
	if(i < 50 || i > 100)
	setInternal("gl_aspect", "100");

	s = getString("gl_fsmax");
	if(s != "never" && s != "ui" && s != "tia" && s != "always")
	setInternal("gl_fsmax", "never");
#endif

#ifdef SOUND_SUPPORT
	i = getInt("volume");
	if(i < 0 || i > 100)
	setInternal("volume", "100");
	i = getInt("freq");
	if(i < 0 || i > 48000)
	setInternal("freq", "31400");
	i = getInt("tiafreq");
	if(i < 0 || i > 48000)
	setInternal("tiafreq", "31400");
#endif

	i = getInt("zoom_ui");
	if (i < 1 || i > 10)
		setInternal("zoom_ui", "2");

	i = getInt("zoom_tia");
	if (i < 1 || i > 10)
		setInternal("zoom_tia", "2");

	i = getInt("paddle");
	if (i < 0 || i > 3)
		setInternal("paddle", "0");

	i = getInt("pthresh");
	if (i < 400)
		setInternal("pthresh", "400");
	else if (i > 800)
		setInternal("pthresh", "800");

	s = getString("prlette");
	if (s != "standard" && s != "z26" && s != "user")
		setInternal("prlette", "standard");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::usage() {

	cerr
			<< "\n"
			" ***************************************************************************\n"
			" * Welcome to A.L.E (Arcade Learning Environment)\n"
			" * (Powered by LibRetro)\n"
			" ***************************************************************************\n"
			"\n"
			" Usage: rle [options ...] -core_file corefile romfile\n"
			"\n"
			" Main arguments:\n"
			"   -help -- prints out help information\n"
			"   -game_controller [fifo|fifo_named"
#ifdef __USE_RLGLUE
			"|rlglue"
#endif
			"] (default: unset)\n"
			"      Defines how Stella communicates with the player agent:\n"
			"            - 'fifo':       Control occurs through FIFO pipes\n"
			"            - 'fifo_named': Control occurs through named FIFO pipes\n"
#ifdef __USE_RLGLUE
			"            - 'rlglue':     External control via RL-Glue\n"
#endif
			"   -random_seed [n|time] (default: time)\n"
			"     Sets the seed used for random number generation\n"
#ifdef __USE_SDL
			"   -display_screen [true|false] (default: false)\n"
			"     Displays the game screen\n"
#endif
#ifdef SOUND_SUPPORT
			"   -sound [true|false] (default: false)\n"
			"     Enable game sounds\n"
#endif
			"\n"
			" Environment arguments:\n"
			"   -max_num_frames m (default: 0)\n"
			"     The program will quit after this number of frames. 0 means never.\n"
			"   -max_num_frames_per_episode m (default: 0)\n"
			"     Ends each episode after this number of frames. 0 means never.\n"
			"   -color_averaging [true|false] (default: false)\n"
			"     Phosphor blends screens to reduce flicker\n"
			"   -record_screen_dir [save_directory]\n"
			"     Saves game screen images to save_directory\n"
			"   -repeat_action_probability (default: 0.25)\n"
			"     Stochasticity in the environment. It is the probability the previous "
			"action will repeated without executing the new one.\n"
			"\n"
			" FIFO Controller arguments:\n"
			"   -run_length_encoding [true|false] (default: true)\n"
			"     Encodes data using run-length encoding\n"
			"\n"
#ifdef __USE_RLGLUE
			" RL-Glue Controller arguments:\n"
			"   -send_rgb [true|false] (default: false)\n"
			"     Sends RGB values for each pixel instead of the pallette index values\n"
			"   -restricted_action_set [true|false] (default: false)\n"
			"     Agents use a smaller set of actions (RL-Glue interfaces only)\n"
			"\n"
#endif
			" Misc. arguments:\n"
			"   -ld [A/B] (default: B)\n"
			"     Left player difficulty. B means easy.\n"
			"   -rd [A/B] (default: B)\n"
			"     Right player difficulty. B means easy.\n"
			"\n"; // Closing the std::cerr statement
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::saveConfig() {
	// Do a quick scan of the internal settings to see if any have
	// changed.  If not, we don't need to save them at all.
	bool settingsChanged = false;
	for (unsigned int i = 0; i < myInternalSettings.size(); ++i) {
		if (myInternalSettings[i].value != myInternalSettings[i].initialValue) {
			settingsChanged = true;
			break;
		}
	}

	if (!settingsChanged)
		return;

	ofstream out(myRleSystem->configFile().c_str());
	if (!out || !out.is_open()) {
		rle::Logger::Error << "Error: Couldn't save settings file\n";
		return;
	}

	out << ";  Stella configuration file" << endl << ";" << endl
			<< ";  Lines starting with ';' are comments and are ignored."
			<< endl << ";  Spaces and tabs are ignored." << endl << ";" << endl
			<< ";  Format MUST be as follows:" << endl << ";    command = value"
			<< endl << ";" << endl
			<< ";  Commmands are the same as those specified on the commandline,"
			<< endl << ";  without the '-' character." << endl << ";" << endl
			<< ";  Values are the same as those allowed on the commandline."
			<< endl
			<< ";  Boolean values are specified as 1 (or true) and 0 (or false)"
			<< endl << ";" << endl;

	// Write out each of the key and value pairs
	for (unsigned int i = 0; i < myInternalSettings.size(); ++i) {
		out << myInternalSettings[i].key << " = " << myInternalSettings[i].value
				<< endl;
	}

	out.close();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setInt(const string& key, const int value) {
	std::ostringstream stream;
	stream << value;

	if (int idx = getInternalPos(key) != -1) {
		setInternal(key, stream.str(), idx);
	} else {
		verifyVariableExistence(intSettings, key);
		setExternal(key, stream.str());
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setFloat(const string& key, const float value) {
	std::ostringstream stream;
	stream << value;

	if (int idx = getInternalPos(key) != -1) {
		setInternal(key, stream.str(), idx);
	} else {
		verifyVariableExistence(floatSettings, key);
		setExternal(key, stream.str());
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setBool(const string& key, const bool value) {
	std::ostringstream stream;
	stream << value;

	if (int idx = getInternalPos(key) != -1) {
		setInternal(key, stream.str(), idx);
	} else {
		verifyVariableExistence(boolSettings, key);
		setExternal(key, stream.str());
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setString(const string& key, const string& value) {
	if (int idx = getInternalPos(key) != -1) {
		setInternal(key, value, idx);
	} else {
		verifyVariableExistence(stringSettings, key);
		setExternal(key, value);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::getSize(const string& key, int& x, int& y) const {
	string size = getString(key);
	replace(size.begin(), size.end(), 'x', ' ');
	istringstream buf(size);
	buf >> x;
	buf >> y;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::getInt(const string& key, bool strict) const {
	// Try to find the named setting and answer its value
	int idx = -1;
	if ((idx = getInternalPos(key)) != -1) {
		return (int) atoi(myInternalSettings[idx].value.c_str());
	} else {
		if ((idx = getExternalPos(key)) != -1) {
			return (int) atoi(myExternalSettings[idx].value.c_str());
		} else {
			if (strict) {
				rle::Logger::Error << "No value found for key: " << key << ". ";
				rle::Logger::Error
						<< "Make sure all the settings files are loaded."
						<< endl;
				exit(-1);
			} else {
				return -1;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
float Settings::getFloat(const string& key, bool strict) const {
	// Try to find the named setting and answer its value
	int idx = -1;
	if ((idx = getInternalPos(key)) != -1) {
		return (float) atof(myInternalSettings[idx].value.c_str());
	} else {
		if ((idx = getExternalPos(key)) != -1) {
			return (float) atof(myExternalSettings[idx].value.c_str());
		} else {
			if (strict) {
				rle::Logger::Error << "No value found for key: " << key << ". ";
				rle::Logger::Error
						<< "Make sure all the settings files are loaded."
						<< endl;
				exit(-1);
			} else {
				return -1.0;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Settings::getBool(const string& key, bool strict) const {
	// Try to find the named setting and answer its value
	int idx = -1;
	if ((idx = getInternalPos(key)) != -1) {
		const string& value = myInternalSettings[idx].value;
		if (value == "1" || value == "true" || value == "True")
			return true;
		else if (value == "0" || value == "false" || value == "False")
			return false;
		else
			return false;
	} else if ((idx = getExternalPos(key)) != -1) {
		const string& value = myExternalSettings[idx].value;
		if (value == "1" || value == "true")
			return true;
		else if (value == "0" || value == "false")
			return false;
		else
			return false;
	} else {
		if (strict) {
			rle::Logger::Error << "No value found for key: " << key << ". ";
			rle::Logger::Error << "Make sure all the settings files are loaded."
					<< endl;
			exit(-1);
		} else {
			return false;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const string& Settings::getString(const string& key, bool strict) const {
	// Try to find the named setting and answer its value
	int idx = -1;
	if ((idx = getInternalPos(key)) != -1) {
		return myInternalSettings[idx].value;
	} else if ((idx = getExternalPos(key)) != -1) {
		return myExternalSettings[idx].value;
	} else {
		if (strict) {
			rle::Logger::Error << "No value found for key: " << key << ". ";
			rle::Logger::Error << "Make sure all the settings files are loaded."
					<< endl;
			exit(-1);
		} else {
			static std::string EmptyString("");
			return EmptyString;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setSize(const string& key, const int value1, const int value2) {
	std::ostringstream buf;
	buf << value1 << "x" << value2;
	setString(key, buf.str());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::getInternalPos(const string& key) const {
	for (unsigned int i = 0; i < myInternalSettings.size(); ++i)
		if (myInternalSettings[i].key == key)
			return i;

	return -1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::getExternalPos(const string& key) const {
	for (unsigned int i = 0; i < myExternalSettings.size(); ++i)
		if (myExternalSettings[i].key == key)
			return i;

	return -1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::setInternal(const string& key, const string& value, int pos,
		bool useAsInitial) {
	int idx = -1;

	if (pos != -1 && pos >= 0 && pos < (int) myInternalSettings.size()
			&& myInternalSettings[pos].key == key) {
		idx = pos;
	} else {
		for (unsigned int i = 0; i < myInternalSettings.size(); ++i) {
			if (myInternalSettings[i].key == key) {
				idx = i;
				break;
			}
		}
	}

	if (idx != -1) {
		myInternalSettings[idx].key = key;
		myInternalSettings[idx].value = value;
		if (useAsInitial)
			myInternalSettings[idx].initialValue = value;

		/*cerr << "modify internal: key = " << key
		 << ", value  = " << value
		 << ", ivalue = " << myInternalSettings[idx].initialValue
		 << " @ index = " << idx
		 << endl;*/
	} else {
		Setting setting;
		setting.key = key;
		setting.value = value;
		if (useAsInitial)
			setting.initialValue = value;

		myInternalSettings.push_back(setting);
		idx = myInternalSettings.size() - 1;

		/*cerr << "insert internal: key = " << key
		 << ", value  = " << value
		 << ", ivalue = " << setting.initialValue
		 << " @ index = " << idx
		 << endl;*/
	}

	return idx;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::setExternal(const string& key, const string& value, int pos,
		bool useAsInitial) {
	int idx = -1;

	if (pos != -1 && pos >= 0 && pos < (int) myExternalSettings.size()
			&& myExternalSettings[pos].key == key) {
		idx = pos;
	} else {
		for (unsigned int i = 0; i < myExternalSettings.size(); ++i) {
			if (myExternalSettings[i].key == key) {
				idx = i;
				break;
			}
		}
	}

	if (idx != -1) {
		myExternalSettings[idx].key = key;
		myExternalSettings[idx].value = value;
		if (useAsInitial)
			myExternalSettings[idx].initialValue = value;

		/*cerr << "modify external: key = " << key
		 << ", value = " << value
		 << " @ index = " << idx
		 << endl;*/
	} else {
		Setting setting;
		setting.key = key;
		setting.value = value;
		if (useAsInitial)
			setting.initialValue = value;

		myExternalSettings.push_back(setting);
		idx = myExternalSettings.size() - 1;

		/*cerr << "insert external: key = " << key
		 << ", value = " << value
		 << " @ index = " << idx
		 << endl;*/
	}

	return idx;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::Settings(const Settings&) {
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings& Settings::operator =(const Settings&) {
	assert(false);

	return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setDefaultSettings() {

	// Controller settings
	intSettings.emplace(std::make_pair("max_num_frames", 0));
	intSettings.emplace(std::make_pair("max_num_frames_per_episode", 0));

	// FIFO controller settings
	boolSettings.emplace(std::make_pair("run_length_encoding", true));

	// Environment customization settings
	boolSettings.emplace(std::make_pair("restricted_action_set", false));
	intSettings.emplace(std::make_pair("random_seed", 0));
	boolSettings.emplace(std::make_pair("color_averaging", true));
	boolSettings.emplace(std::make_pair("send_rgb", false));
	intSettings.emplace(std::make_pair("frame_skip", 1));
	floatSettings.emplace(std::make_pair("repeat_action_probability", 0.25));
	stringSettings.emplace(std::make_pair("rom_file", ""));
	stringSettings.emplace(std::make_pair("core_file", ""));
	boolSettings.emplace(std::make_pair("two_players", false));

	// Record settings
	intSettings.emplace(std::make_pair("fragsize", 64)); // fragsize to 64 ensures proper sound sync
	stringSettings.emplace(std::make_pair("record_screen_dir", ""));
	stringSettings.emplace(std::make_pair("record_sound_filename", ""));

	// Display Settings
	boolSettings.emplace(std::make_pair("display_screen", false));

	// Game-Specific Settings

		// Mortal Kombat
	boolSettings.emplace(std::make_pair("MK_random_position", true));
	stringSettings.emplace(std::make_pair("MK_difficulty", "medium"));
	stringSettings.emplace(std::make_pair("MK_player1_character", "cage"));
	stringSettings.emplace(std::make_pair("MK_player2_character", "scorpion"));
	intSettings.emplace(std::make_pair("MK_opponent_character", 0));

	for (map<string, string>::iterator it = stringSettings.begin();
			it != stringSettings.end(); it++) {
		this->setString(it->first, it->second);
	}

	for (map<string, float>::iterator it = floatSettings.begin();
			it != floatSettings.end(); it++) {
		this->setFloat(it->first, it->second);
	}

	for (map<string, bool>::iterator it = boolSettings.begin();
			it != boolSettings.end(); it++) {
		this->setBool(it->first, it->second);
	}

	for (map<string, int>::iterator it = intSettings.begin();
			it != intSettings.end(); it++) {
		this->setInt(it->first, it->second);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<typename ValueType>
void Settings::verifyVariableExistence(map<string, ValueType> dict,
		string key) {
	if (dict.find(key) == dict.end()) {
		throw std::runtime_error(
				"The key " + key + " you are trying to set does not exist.\n");
	}
}
