#include "SumerEngine.h"

int wmain(int argc, wchar_t** argv, wchar_t** envp) {
	SumerEngine sEngine(960, 540);
	sEngine.InitEngine();
	while (sEngine.LoopSetUp()) {

	}
	return 0;
}