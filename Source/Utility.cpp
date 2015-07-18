#include "Utility.h"

#include <iostream>
#include <unistd.h>

namespace util
{

bool ParseIntFromJsonString( const std::string &jsonStringIn, const std::string &keyIn, int &valueOut )
{
	// Parse the json string
	rapidjson::Document document;
	document.Parse( jsonStringIn.c_str() );

	// Make sure string was correctly parsed
	if( !document.IsObject() )						{ return false; }
	if( !document.HasMember( keyIn.c_str() ) )		{ return false; }
	if( !document[ keyIn.c_str() ].IsInt() )		{ return false; }

	// Get the port number
	valueOut = document[ keyIn.c_str() ].GetInt();

	return true;
}

bool ReadFramesFromFile(const std::string& fileName, std::vector<rebop::commands::bebop::video::TRawFrame>& frames)
{
	auto file = fopen(fileName.c_str(), "rb");
	if (file == NULL) { LOG(ERROR) << "Failed to open file"; return false; }

	// obtain file size:
	fseek (file , 0 , SEEK_END);
	auto linesInFile = ftell (file);
	rewind (file);
	if (linesInFile < 1) { LOG(WARNING) << "File is empty"; return true; }

	// allocate memory to contain the whole file:
	uint8_t* buffer = (uint8_t*) malloc (sizeof(uint8_t)*linesInFile);
	if (buffer == NULL) { LOG(ERROR) << "Failed to allocate space"; return false; }

	// copy the file into the buffer:
	auto linesRead = fread (buffer,1, linesInFile, file);
	if (linesRead != linesInFile) { LOG(ERROR) << "File was not read correctly"; return false; }

	// Insert buffer into array of frames
	for (size_t i = 0; i < linesInFile; i++)
	{
		uint8_t* frameData = &buffer[i];
		uint32_t frameDataSize(sizeof(buffer[i]));
		frames.push_back(rebop::commands::bebop::video::TRawFrame(frameData, frameDataSize, 0, 0));
	}
	fclose(file);
	free (buffer);

	return true;
}

bool WriteFramesToFile(const std::string& fileName, const std::vector<rebop::commands::bebop::video::TRawFrame>& frames)
{
	auto file = fopen(fileName.c_str(), "w");
	if (file == NULL) { LOG(ERROR) << "Failed to open file"; return false; }

	for (const auto& frame : frames)
	{
		fwrite(frame.GetRawData(), frame.GetRawFrameDataSize(), 1, file);
		fflush (file);
	}
	fclose(file);

	return true;
}

}
