/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2010  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "IsoFileDescriptor.h"
#include "SectorSource.h"

#include "common/Pcsx2Defs.h"
#include <string_view>

class Error;

class IsoFile final
{
public:
	static const int sectorLength = 2048;

protected:
	SectorSource& internalReader;
	IsoFileDescriptor fileEntry = {};

	u32 currentOffset = 0;
	u32 maxOffset = 0;

	int currentSectorNumber = 0;
	int sectorOffset = 0;
	u8 currentSector[sectorLength];

public:
	IsoFile(SectorSource& reader);
	IsoFile(SectorSource& reader, const IsoFileDescriptor& fileEntry);
	~IsoFile();

	bool open(const IsoDirectory& dir, const std::string_view& filename, Error* error = nullptr);
	bool open(const std::string_view& filename, Error* error = nullptr);

	u32 seek(u32 absoffset);
	u32 seek(s64 offset, int mode);
	void reset();

	s32 skip(s32 n);
	u32 getSeekPos() const;
	u32 getLength();
	bool eof() const;

	const IsoFileDescriptor& getEntry();

	u8 readByte();
	s32 read(void* dest, s32 len);
	std::string readLine();

	// Tool to read a specific value type, including structs.
	template <class T>
	T read()
	{
		if (sizeof(T) == 1)
			return (T)readByte();
		else
		{
			T t;
			read((u8*)&t, sizeof(t));
			return t;
		}
	}

protected:
	void makeDataAvailable();
	int internalRead(void* dest, int off, int len);
	void Init();
};
