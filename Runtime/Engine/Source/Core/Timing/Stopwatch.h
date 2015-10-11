#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace Blueshift {
	namespace Core {
		namespace Timing {

			class IStopwatch {
			public:
				virtual void Start() = 0;
				virtual void Stop() = 0;

				virtual double GetElapsedSeconds() = 0;
			};

#ifdef _WIN32
			class Stopwatch {
			private:
				bool running;
				double elapsed;

				LARGE_INTEGER frequency;
				LARGE_INTEGER start;
			public:
				inline Stopwatch()
					: running(false), elapsed(0.0) {
					QueryPerformanceFrequency(&frequency);
					QueryPerformanceCounter(&start);
				}
				inline Stopwatch(bool ShouldRun)
					: running(ShouldRun), elapsed(0.0), frequency(), start() {
					QueryPerformanceFrequency(&frequency);
					QueryPerformanceCounter(&start);
				}

				virtual void Start() {
					running = true;
					QueryPerformanceCounter(&start);
				}

				virtual void Stop() {
					LARGE_INTEGER stop;
					QueryPerformanceCounter(&stop);
					elapsed = (double)(stop.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
					running = false;
				}

				virtual double GetElapsedSeconds() {
					if (running) {
						//If running, momentarily stop to get the elapsed time
						Stop();
						//Then keep it going.
						running = true;
					}
					//Now this is valid no matter what.
					return elapsed;
				}

			};
#endif
		}
	}
}