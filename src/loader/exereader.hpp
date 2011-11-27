
#ifndef INNOEXTRACT_LOADER_EXEREADER_HPP
#define INNOEXTRACT_LOADER_EXEREADER_HPP

#include <stdint.h>
#include <istream>
#include <vector>

namespace loader {

/*!
 * Minimal PE/COFF parser that can find resources by ID in .exe files.
 * This implementation is optimized to look for exactly one resource.
 */
class exe_reader {
	
public:
	
	struct resource {
		
		uint32_t offset; //! File offset of the resource data in bytes.
		
		uint32_t size; //! Size of the resource data in bytes.
		
	};
	
	enum resource_id {
		TypeData = 10,
		LanguageDefault = 0
	};
	
	/*!
	 * Find where a resource with a given ID is stored in a MS PE/COFF executable.
	 * @return The location of the resource or (0, 0) if the requested resource does not exist.
	 */
	static resource find_resource(std::istream & is, uint32_t name, uint32_t type = TypeData,
	                              uint32_t language = LanguageDefault);
	
private:
	
	struct header;
	struct section;
	
	typedef std::vector<section> section_list;
	
	/*!
	 * Find the entry in a resource table with a given ID.
	 * The input stream is expected to be positioned at the start of the table.
	 * The position if the stream after the function call is undefined.
	 * 
	 * @return:
	 *   Highest order bit: 1 = points to another resource table
	 *                      0 = points to a resource leaf
	 *   Remaining 31 bits: Offset to the resource table / leaf relative to
	 *                      the directory start.
	 */
	static uint32_t find_resource_entry(std::istream & is, uint32_t id);
	
	static bool load_header(std::istream & is, header & coff);
	
	static bool load_section_list(std::istream & is, const header & coff, section_list & table);
	
	/*!
	 * Convert a memory address to a file offset according to the given section list.
	 */
	static uint32_t to_file_offset(const section_list & sections, uint32_t address);
	
};

} // namespace loader

#endif // INNOEXTRACT_LOADER_EXEREADER_HPP
