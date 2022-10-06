#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <string>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mlock;

long long threshold = 6;
unsigned long long gcycles = 0;

struct best {
	long long best;
	long long n;
	long long m;
	long long e;
	long long r;
};

best gbest;

struct format {
	long double num;
	std::string symbol;
};

format format_seconds(long double num) {
	format f;
	
	if (num < 60) { f.num = num; f.symbol = "s"; }
	if (num > 60 && num < 3600) { f.num = num / 60; f.symbol = "m"; }
	if (num > 3600) { f.num = num / 3600; f.symbol = "h"; }

	return f;
}

format format_long(unsigned long long num) {
	format f;

	if (num < 1000) { f.num = num; f.symbol = ""; return f;	}
	if (num > 1000 && num < 1000000) { f.num = num / 1000.0f; f.symbol = "k"; return f;	}
	if (num > 1000000 && num < 1000000000) { f.num = num / 1000000.0f; f.symbol = "m"; return f;	}
	if (num > 1000000000) { f.num = num / 1000000000.0f; f.symbol = "b"; return f;	}

	return f;
}

void print_square(long long n, long long m, long long e) {
	long long a, b, c, d, f, g, h, i;
	bool as{}, bs{}, cs{}, ds{}, es{}, fs{}, gs{}, hs{}, is{};

	a = e + n;
	b = e - n - m;
	c = e + m;
	d = e - n + m;
	f = e + n - m;
	g = e - m;
	h = e + n + m;
	i = e - n;

	auto square = [](long double x) {
		if (x > 0) {
			long long sr = sqrt(x);
			return (sr * sr == x);
		} return false;
	};

	if (square(a)) { as = 1; }
	if (square(b)) { bs = 1; }
	if (square(c)) { cs = 1; }
	if (square(d)) { ds = 1; }
	if (square(e)) { es = 1; }
	if (square(f)) { fs = 1; }
	if (square(g)) { gs = 1; }
	if (square(h)) { hs = 1; }
	if (square(i)) { is = 1; }

	if (as) { std::cout << "*"; } std::cout << a << ":";
	if (bs) { std::cout << "*"; } std::cout << b << ":";
	if (cs) { std::cout << "*"; } std::cout << c << "\n";
	if (ds) { std::cout << "*"; } std::cout << d << ":";
	if (es) { std::cout << "*"; } std::cout << e << ":";
	if (fs) { std::cout << "*"; } std::cout << f << "\n";
	if (gs) { std::cout << "*"; } std::cout << g << ":";
	if (hs) { std::cout << "*"; } std::cout << h << ":";
	if (is) { std::cout << "*"; } std::cout << i << "\n";
	std::cout << "n:" << n << " m:" << m << "\n\n";

}

long long check_square(long long n, long long m, long long e) {
	long long a, b, c, d, f, g, h, i;
	bool as{}, bs{}, cs{}, ds{}, es{}, fs{}, gs{}, hs{}, is{};
	
	a = e + n;
	b = e - n - m;
	c = e + m;
	d = e - n + m;
	f = e + n - m;
	g = e - m;
	h = e + n + m;
	i = e - n;

	long long matches = 0;

	auto square = [](long double x) {
		if (x > 0) {
			long long sr = sqrt(x);
			return (sr * sr == x);
		} return false;
	};

	if (square(a)) { matches++; as = 1; }
	if (square(b)) { matches++; bs = 1; }
	if (square(c)) { matches++; cs = 1; }
	if (square(d)) { matches++; ds = 1; }
	if (square(e)) { matches++; es = 1; }
	if (square(f)) { matches++; fs = 1; }
	if (square(g)) { matches++; gs = 1; }
	if (square(h)) { matches++; hs = 1; }
	if (square(i)) { matches++; is = 1; }

	if (matches > threshold) {
		
		if (as) { std::cout << "*"; } std::cout << a << ":";
		if (bs) { std::cout << "*"; } std::cout << b << ":";
		if (cs) { std::cout << "*"; } std::cout << c << "\n";
		if (ds) { std::cout << "*"; } std::cout << d << ":";
		if (es) { std::cout << "*"; } std::cout << e << ":";
		if (fs) { std::cout << "*"; } std::cout << f << "\n";
		if (gs) { std::cout << "*"; } std::cout << g << ":";
		if (hs) { std::cout << "*"; } std::cout << h << ":";
		if (is) { std::cout << "*"; } std::cout << i << "\n";
		std::cout << "n:" << n << " m:" << m << " matches:" << matches << "\n\n";
	}

	return matches;
}

static int thr_find_from_r(long long r, long long step, long long offset) {
	best ffr;
	format form;

	long long a, b, c, d, e, f, g, h, i;
	long long n, m;

	e = r * r;
	long long rlimit = sqrt(e) * 2;

	unsigned long long cycles = 0;
	long long validnm = 0;
	long long validai = 0;
	long long matches = 0;
	long long best = 0;
	long long bestn = 0;
	long long bestm = 0;
	long long beste = 0;

	std::chrono::high_resolution_clock::time_point ffr1 = std::chrono::high_resolution_clock::now();

	for (long long fn = 1; fn < rlimit; fn += step) {
		for (long long fm = 1; fm < rlimit; fm++) {

			a = fn * fn;
			c = fm * fm;

			n = a - e;
			m = c - e;

			if (n > 0 && m > 0 && n != m) {
				//validnm++;

				a = e + n;
				b = e - n - m;
				c = e + m;
				d = e - n + m;
				f = e + n - m;
				g = e - m;
				h = e + n + m;
				i = e - n;

				if (a > 0 && b > 0 && c > 0 && d > 0 && f > 0 && g > 0 && h > 0 && i > 0) {

					matches = check_square(n, m, e);
					if (matches >= best) {
						best = matches;
						bestn = n;
						bestm = m;
						beste = e;
					}

					//validai++;
				}
			}

			cycles++;
		}
	}

	std::chrono::high_resolution_clock::time_point ffr2 = std::chrono::high_resolution_clock::now();
	auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(ffr2 - ffr1);

	mlock.lock();

	ffr.best = best;
	ffr.n = bestn;
	ffr.m = bestm;
	ffr.e = beste;
	ffr.r = sqrt(ffr.e);

	gcycles += cycles;

	if (ffr.best >= gbest.best) {
		gbest = ffr;
	}

	mlock.unlock();

	return 1;

	long cps = cycles / total_time.count();

	std::cout << "r:" << r << " cycles:" << cycles << "\n";
	std::cout << "time:" << total_time.count() << "s cps:" << cps / 1000000 << "m \n";
	std::cout << "validnm:" << validnm << " validai:" << validai << "\n";
	std::cout << "best:" << best << " bestn:" << bestn << " bestm:" << bestm << "\n\n";

}
 
int main()
{
	best ffrbest{};
	
	gbest.best = 0;
	
	std::cout << std::setprecision(2);
	std::cout.setf(std::ios::fixed, std::ios::floatfield);

	long long from = 0;
	long long to = 0;
	long long gnumthreads = 15;

start:
	gbest.best = 0;
	

	std::cout << "from:";
	std::cin >> from;
	if (from == 0) { return 0; }

	std::cout << "to:";
	std::cin >> to;
	if (to == 0) { return 0; }
	
	for (long long r = from; r < to; r ++) {

		gcycles = 0;

		std::chrono::high_resolution_clock::time_point ffr1 = std::chrono::high_resolution_clock::now();

		std::vector<std::thread> thr(gnumthreads);

		for (int offset = 0; offset < gnumthreads; offset++) {
			thr[offset] = std::thread(thr_find_from_r, r, gnumthreads, offset);
		}

		for (int i = 0; i < gnumthreads; i++) {
			thr[i].join();
		}

		std::chrono::high_resolution_clock::time_point ffr2 = std::chrono::high_resolution_clock::now();
		auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(ffr2 - ffr1);

		unsigned long long cps = gcycles / total_time.count();

		std::cout << "best:" << gbest.best << " r:" << gbest.r <<
			" time:" << format_seconds(total_time.count()).num << format_seconds(total_time.count()).symbol <<
			" cycles:" << format_long(gcycles).num << format_long(gcycles).symbol <<
			" cps:" << format_long(cps).num << format_long(cps).symbol << "\n";

		//print_square(gbest.n, gbest.m, gbest.e);

		if (gbest.best >= ffrbest.best) {
			ffrbest = gbest;
		}
	}

	std::cout << "f.best:" << ffrbest.best << " f.r:" << ffrbest.r << "\n\n";
	print_square(ffrbest.n, ffrbest.m, ffrbest.e);

	goto start;

	return 0;
}