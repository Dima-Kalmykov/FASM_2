#include <iostream>
#include <vector>
#include <iomanip>
#include <tuple>
#include <thread>
#include <random>
#include <mutex>
#include <string>
#include <fstream>

bool exist_coplanar_triplet = false;

// Error for comparing double value with 0.
const double ACCURACY = 1e-10;

/// <summary>
/// Vector.
/// </summary>
struct Vector {
	double x;
	double y;
	double z;
};

/// <summary>
/// Triplet of vectors.
/// </summary>
struct Triplet {
	Vector v1;
	Vector v2;
	Vector v3;
};

/// <summary>
/// Override operator << for output vectors.
/// </summary>
/// <param name="out"> Output stream </param>
/// <param name="vector"> Vector for output </param>
/// <returns> Reference to output stream </returns>
std::ostream& operator<<(std::ostream& out, const Vector& vector) {
	out << std::setprecision(3) << "(" << vector.x << " " << vector.y << " " << vector.z << ")";

	return out;
}

/// <summary>
/// Override operator << for output triplet of vectors.
/// </summary>
/// <param name="out"> Output stream </param>
/// <param name="vector"> Vector for output </param>
/// <returns> Reference to ouput stream </returns>
std::ostream& operator<<(std::ostream& out, const Triplet& vector) {
	out << "{" << vector.v1 << ", " << vector.v2 << ", " << vector.v3 << "}";

	return out;
}

/// <summary>
/// Helper method to simplify calculations.
/// </summary>
double GetMinorDeterminant(double a1, double b2, double b3, double c2, double c3) {
	return a1 * (b2 * c3 - c2 * b3);
}

/// <summary>
/// Check triplet of vector for coplanar.
/// </summary>
/// <param name="triplet"> Triplet of vectors </param>
/// <returns> True if vectors are coplanar, false - otherwise </returns>
bool AreCoplanarTriplet(const Triplet& triplet) {
	const auto v1 = triplet.v1;
	const auto v2 = triplet.v2;
	const auto v3 = triplet.v3;

	return abs(GetMinorDeterminant(v1.x, v2.y, v2.z, v3.y, v3.z) -
		GetMinorDeterminant(v1.y, v2.x, v2.z, v3.x, v3.z) +
		GetMinorDeterminant(v1.z, v2.x, v2.y, v3.x, v3.y)) < ACCURACY;
}

/// <summary>
/// Get all triplets of vectors.
/// </summary>
/// <param name="vectors"> All vectors </param>
/// <returns> All triplets of vectors </returns>
std::vector<Triplet> GetAllTriplets(std::vector<Vector>& vectors) {
	std::vector<Triplet> triplets;
	const int size = vectors.size();

	for (auto i = 0; i < size; ++i) {
		for (auto j = i + 1; j < size; ++j) {
			for (auto k = j + 1; k < size; ++k) {
				triplets.emplace_back(Triplet{ vectors[i], vectors[j], vectors[k] });
			}
		}
	}

	return triplets;
}

/// <summary>
/// Split string by tokens and convert tokens to double.
/// </summary>
/// <param name="line"> Line for splitting </param>
/// <returns> List of double </returns>
std::vector<double> Split(std::string& line) {
	size_t pos;
	std::vector<double> result;

	while ((pos = line.find(' ')) != std::string::npos) {
		auto token = line.substr(0, pos);
		result.emplace_back(stod(token));
		line.erase(0, pos + 1);
	}

	result.emplace_back(stod(line));

	return result;
}

/// <summary>
/// Get list of vectors from file.
/// </summary>
/// <param name="file_path"> Path to file </param>
/// <returns> List of vectors </returns>
std::vector<Vector> GetVectorsFromFile(const std::string& file_path) {
	std::ifstream input(file_path);
	std::vector<Vector> vectors;
	std::string current_line;
	std::string copy;

	try
	{
		while (std::getline(input, current_line)) {
			copy = current_line;
			auto values = Split(current_line);
			vectors.emplace_back(Vector{ values[0], values[1], values[2] });
		}
	} catch (...) {
		std::cout << "\nWrong format of input data on line \"" + copy + 
			"\"!\nVector should be represents as 3 doubles x y z separated by space\n";
		std::cout << "Please, fix problem and restart the app\n";
		exit(1);
	}
	
	return vectors;
}

/// <summary>
/// Filter triplets.
/// </summary>
/// <param name="triplets"> List of triplets </param>
/// <param name="file_path"> Path to file </param>
void FilterTriplets(const std::vector<Triplet>& triplets, const std::string& file_path) {
	std::mutex mutex;
	mutex.lock();
	std::ofstream output(file_path, std::ios_base::app);
	for (const auto& triplet : triplets) {
		if (AreCoplanarTriplet(triplet)) {
			exist_coplanar_triplet = true;
			output << triplet << "\n";
		}
	}
	mutex.unlock();
}

/// <summary>
/// Get part of vector (1/4, 2/4, etc).
/// </summary>
/// <param name="triplets"> List of triplets </param>
/// <param name="part"> Part of vector (from 4) </param>
/// <returns> Iterator to begin of part of vector </returns>
auto GetVectorPartIterator(std::vector<Triplet>& triplets, const int part) {
	return begin(triplets) + triplets.size() / 4 * part;
}

/// <summary>
/// Run threads to filter triplets.
/// </summary>
/// <param name="triplets"> List of triplets </param>
/// <param name="file_path"> Path to file </param>
void RunThreads(std::vector<Triplet>& triplets, const std::string& file_path) {
	std::vector<Triplet> part1(begin(triplets), GetVectorPartIterator(triplets, 1));
	std::vector<Triplet> part2(GetVectorPartIterator(triplets, 1), GetVectorPartIterator(triplets, 2));
	std::vector<Triplet> part3(GetVectorPartIterator(triplets, 2), GetVectorPartIterator(triplets, 3));
	std::vector<Triplet> part4(GetVectorPartIterator(triplets, 3), end(triplets));

	std::vector<std::thread> threads;

	threads.emplace_back(std::thread(FilterTriplets, part1, file_path));
	threads.emplace_back(std::thread(FilterTriplets, part2, file_path));
	threads.emplace_back(std::thread(FilterTriplets, part3, file_path));
	threads.emplace_back(std::thread(FilterTriplets, part4, file_path));

	for (auto& thr : threads) {
		thr.join();
	}
}

/// <summary>
/// Get number of test.
/// </summary>
/// <returns> Number of test (between 1 and 5 inclusively) </returns>
int GetTestNumber() {
	auto test_number = 0;
	std::cout << "Enter number of test (1-5): ";
	while (true) {
		try {
			std::string value;
			std::cin >> value;

			test_number = std::stoi(value);
			if (test_number > 5 || test_number < 1) {
				std::cout << "Wrong input, enter number in range [1, 5]: ";
				continue;
			}

			return test_number;
		}
		catch (...) {
			std::cout << "Wrong input, enter number in range [1, 5]: ";
		}
	}
}

int main() {
	std::ios::sync_with_stdio(false);

	const auto test_number = GetTestNumber();

	const auto input_path = "test" + std::to_string(test_number) + ".txt";
	const auto output_path = "answer" + std::to_string(test_number) + ".txt";

	auto vectors = GetVectorsFromFile(input_path);

	auto triplets = GetAllTriplets(vectors);

	RunThreads(triplets, output_path);

	if (!exist_coplanar_triplet) {
		std::ofstream output(output_path);

		output << "No coplanar triplets";
	}

	std::cout << "\nSuccessfully!\n";
	return 0;
}
