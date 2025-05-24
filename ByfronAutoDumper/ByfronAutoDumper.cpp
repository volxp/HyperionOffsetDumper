#include "Gay/Dump.hpp"

void startdump() {
    spdlog::warn("Starting Dump...");
	spdlog::info("BitMap: 0x{:X}", Addresses::Hyperion::FUNC::BitMap());
	spdlog::info("PageHash: 0x{:X}", Addresses::Hyperion::FUNC::PageHash());
	spdlog::info("BitMapHash: 0x{:X}", Addresses::Hyperion::FUNC::BitMapHash());
	spdlog::info("MaxCaps: 0x{:X}", Addresses::Hyperion::FUNC::MaxCaps());
	spdlog::info("Dump finished.");
	spdlog::warn("Press any key to exit...");
	std::cin.get();
}


int main() {


	startdump();
    return 0;
}
