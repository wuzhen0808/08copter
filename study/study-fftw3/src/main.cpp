

// #include <pins_arduino.h>
#include <fftw3.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>

#define REAL 0
#define IMAG 1
using namespace std;
int writeGpScript(const char *datName, int rate, int N);
string fname("build/fc231229.csv");
const char *ofname = "build/out.dat";
int N = 50;
const char *COL = "RollPid-err";
int RATE = 5;
ifstream f(fname);

int loadInput(vector<double> *in) {
    string line;
    getline(f, line);
    string name = "";
    vector<string> header;
    for (int i = 0; i < line.length(); i++) {
        char c = line.at(i);
        if (c == ',') {
            header.push_back(name);
            name = "";
        } else {
            name += c;
        }
    }
    int inputCol;
    for (int i = 0; i < header.size(); i++) {
        if (header.at(i) == COL) {
            inputCol = i;
            break;
        }
    }
    if (inputCol == -1) {
        cout << "no such column:" << COL;
        return -1;
    }

    while (std::getline(f, line)) {
        cout << line << endl;
        stringstream ss(line);
        double d;
        int colI = 0;
        while (ss >> d) {
            if (ss.peek() == ',') {
                ss.ignore();
            }
            if (colI == inputCol) {
                break;
            }
            colI++;
        }
        in->push_back(d);
    }
    return in->size();
}

int main() {
    if (!f.good()) {
        cout << "cannot open file:" << fname << endl;
        return -1;
    }
    vector<double> samples;
    int ret = loadInput(&samples);
    if (ret < 0) {
        return -1;
    }
    int totalSample = samples.size();
    cout << "totalSample:" << totalSample << endl;

    ofstream oFile(ofname);
    // fftw_complex *in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    double *in = (double *)fftw_malloc(sizeof(double) * N);

    fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (N / 2 + 1));
    // fftw_plan p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_MEASURE);
    fftw_plan p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_MEASURE);

    cout << "done of plan" << endl;
    for (int i = 0; i < totalSample; i = i + (N - N / 2)) {
        for (int j = 0; j < N; j++) {
            if (i + j < totalSample) {
                // in[j][REAL] = samples.at(i + j);
                // in[j][IMAG] = 0;
                in[j] = samples.at(i + j);
            }
        }
        cout << "before fftw_execute,i:" << i << endl;
        // fftw_plan p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        fftw_execute(p); /* repeat as needed */
        cout << "done of fftw_execute,i:" << i << endl;
        // for (int i = 0; i < N / 2 + 1; i++) {
        //     cout << i << ":(" << out[i][REAL] << "," << out[i][IMAG] << ")" << endl;
        // }

        double correction = (double)RATE / (double)N;
        for (int j = 0; j < N / 2 + 1; j++) {
            double mag = 2 * (out[j][0] * out[j][0] + out[j][1] * out[j][1]) / N;
            double freqBin = (double)(j)*correction;
            double mag2 = (10. * log10(mag + 0.001)) / log10(10);
            oFile << i << " " << freqBin << " " << mag2 << " " << endl;
        }
        oFile << endl;
    }

    writeGpScript(ofname, RATE, N);

    fftw_free(in);
    fftw_free(out);
    fftw_destroy_plan(p);

    fftw_cleanup();
    return 0;
}

int writeGpScript(const char *datName, int rate, int N) {
    cout << "writeGpScript:" << datName << ",rate:" << rate << endl;
    FILE *gnuplotFile = fopen("build/spec.gp", "w");

    fprintf(gnuplotFile, "########\n");
    fprintf(gnuplotFile, "DATFILE = \"%s\"\n", datName);
    fprintf(gnuplotFile, "set yrange[0:%d] #top frequency to show\n", rate / 2);
    fprintf(gnuplotFile, "#set xrange[0:]  #number of seconds to display\n");
    fprintf(gnuplotFile, "########\n");
    fprintf(gnuplotFile, "set terminal png enhanced size 800,600\n");
    fprintf(gnuplotFile, "set view 0,0\n");
    // fprintf(gnuplotFile, "set contour base\n");
    fprintf(gnuplotFile, "set lmargin at screen 0.15\n");
    fprintf(gnuplotFile, "set rmargin at screen 0.85\n");
    fprintf(gnuplotFile, "set bmargin at screen 0.15\n");
    fprintf(gnuplotFile, "set tmargin at screen 0.85\n");
    fprintf(gnuplotFile, "set title DATFILE\n");
    fprintf(gnuplotFile, "set output DATFILE[:strlen(DATFILE)-4].\'.png\'\n");
    fprintf(gnuplotFile, "set ytics rotate\n");
    fprintf(gnuplotFile, "set xlabel \"Time (s)\" offset character 0, 2,0\n");
    fprintf(gnuplotFile, "###\n");
    fprintf(gnuplotFile, "###I couldn't get the label to show =(\n");
    fprintf(gnuplotFile, "###set label 2 \"Frequency (Hz)\"\n");
    fprintf(gnuplotFile, "###\n");
    fprintf(gnuplotFile, "show label\n");
    fprintf(gnuplotFile, "set ytics border mirror\n");
    fprintf(gnuplotFile, "unset ztics\n");
    fprintf(gnuplotFile, "set grid			#Put gridlines on the plot\n");
    // fprintf(gnuplotFile,"set format cb \"\%10.1\f\"\n");
    fprintf(gnuplotFile, "\n");
    fprintf(gnuplotFile, "set cbtics border in scale 0,0 autojustify mirror offset -8,0\n");
    fprintf(gnuplotFile, "set colorbox user\n");
    fprintf(gnuplotFile, "\n");
    fprintf(gnuplotFile, "set colorbox vertical user origin 0.1, 0.29 size 0.01,0.45\n");
    fprintf(gnuplotFile, "#set logscale y 2\n");
    fprintf(gnuplotFile, "color(x) = x    #this just scales down the colorbox numbers\n");
    // fprintf(gnuplotFile,"#set yrange[500:2000]\n");
    fprintf(gnuplotFile, "\n");
    fprintf(gnuplotFile, "splot DATFILE using ($1/%d):2:(color($3)) with pm3d notitle\n", rate);
    fclose(gnuplotFile);
    return 0;
}