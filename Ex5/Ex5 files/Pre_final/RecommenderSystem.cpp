/**
 * @file RecommenderSystem.cpp
 * @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
 * @ID 300575297
 * @date 13 May 2020
 *
 */

// ------------------------------ includes ------------------------------------------

#include "RecommenderSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <chrono> // timing

// ------------------------------ macros & constants --------------------------------

#define MOVIES_BIG "/Users/Muadz/Downloads/HUJI/2019_20_sem_B/C_C++/Exercises/Ex5/" \
				   "Ex5 files/movies_big.txt"
#define MOVIES_SMALL "/Users/Muadz/Downloads/HUJI/2019_20_sem_B/C_C++/Exercises/Ex5/" \
				   "Ex5 files/movies_small.txt"
#define MOVIES_FEATURES "/Users/Muadz/Downloads/HUJI/2019_20_sem_B/C_C++/Exercises/Ex5/" \
				   "Ex5 files/movies_features.txt"
#define RANKS_BIG "/Users/Muadz/Downloads/HUJI/2019_20_sem_B/C_C++/Exercises/Ex5/" \
				   "Ex5 files/ranks_big.txt"
#define RANKS_SMALL "/Users/Muadz/Downloads/HUJI/2019_20_sem_B/C_C++/Exercises/Ex5/" \
				   "Ex5 files/ranks_small.txt"
#define RANKS_MATRIX "/Users/Muadz/Downloads/HUJI/2019_20_sem_B/C_C++/Exercises/Ex5/" \
				   "Ex5 files/ranks_matrix.txt"


#define FILE_NOT_OPENED "Unable to open file "
#define USER_NOT_FOUND "USER NOT FOUND"
#define MOVIE_NOT_FOUND "MOVIE NOT FOUND"


#define NA -25  // NA = Not rated by user
#define NOT_FOUND -1
#define LOAD_FAIL -1
#define LOAD_SUCCESS 0


// ------------------------------ functions implementation ---------------------------


/**
 * @brief
 * @param moviesAttributesFilePath
 * @param userRanksFilePath
 * @return
 */
int RecommenderSystem::loadData (const std::string &moviesAttributesFilePath,
								 const std::string &userRanksFilePath)
{
	/* Loading the movies attributes file */
	std::ifstream moviesAttributesFile;
	moviesAttributesFile.open(moviesAttributesFilePath, std::ios::in);
	if (!moviesAttributesFile)
	{
		std::cerr << FILE_NOT_OPENED << moviesAttributesFilePath << std::endl;
		return LOAD_FAIL;
	}

	std::string movieDetails;
	while (std::getline(moviesAttributesFile,movieDetails))
	{
		std::istringstream issM(movieDetails);
		Movie movieStruct;
		issM >> movieStruct.movieTitle;
		for (int i; issM >> i; )
		{
			movieStruct.attributes.push_back(i);
		}
		_Movies.push_back(movieStruct);
	}
	std::sort(_Movies.begin(), _Movies.end());  // sort _Movies vector
	moviesAttributesFile.close();

	/* Loading the users rankings file */
	std::ifstream userRanksFile;
	userRanksFile.open(userRanksFilePath, std::ios::in);
	if (!userRanksFile)
	{
		std::cerr << FILE_NOT_OPENED << userRanksFilePath << std::endl;
		return LOAD_FAIL;
	}

	std::string userRanks;
	std::getline(userRanksFile,userRanks);
	std::istringstream issU1(userRanks);
	std::vector<std::string> titlesLine;
	for (std::string t; issU1 >> t; )
	{
		titlesLine.push_back(t);
	}

	while (std::getline(userRanksFile,userRanks))
	{
		std::istringstream issU2(userRanks);
		User userStruct;
		issU2 >> userStruct.userName;

		for (const auto & title : titlesLine)
		{
			int j;
			issU2 >> j;
			if (issU2.eof()) { break; }
			if (issU2.fail())
			{
				userStruct.ranks[title][0] = NA;
				issU2.clear();
				issU2.ignore(2, ' ');
			}
			else
			{
				userStruct.ranks[title][0] = j;
			}
			userStruct.ranks[title][1] = NA;
		}
		_Users.push_back(userStruct);
	}
	std::sort(_Users.begin(), _Users.end());  // sort _Users vector
	userRanksFile.close();

	return LOAD_SUCCESS;
}

/**
 * @brief
 * @param userName
 * @return
 */
const std::string RecommenderSystem::recommendByContent (const std::string &userName)
{
	/* Search for userName */
	auto it = std::find_if(_Users.begin(), _Users.end(),
			  [&](const User &user)	{ return user.userName == userName;	});
	if (it == _Users.end())  // User Not Found
	{
		std::cerr << USER_NOT_FOUND << std::endl;
		return USER_NOT_FOUND;
	}

	/* STAGE (1): Calculate the normalized ranks for the user */
	int uIndex = std::distance(_Users.begin(), it);  // index of found userName
	int moviesNum = _Users[uIndex].ranks.size();
	std::cout << "moviesNum = " << moviesNum << std::endl;
	std::cout << _Users[uIndex].userName << ' ';
	double ranksSum = 0;
	int numRated = 0;
	for (const auto& rank : _Users[uIndex].ranks)
	{
		if (rank.second[0] != NA)
		{
			ranksSum += rank.second[0];
			numRated++;
		}
	}
	double ranksAvg = ranksSum / numRated;
	std::cout << "ranksAvg = " << ranksAvg << std::endl;
	for (auto& rank : _Users[uIndex].ranks)
	{
		if (rank.second[0] != NA)
		{
			rank.second[1] = rank.second[0] - ranksAvg;
		}
	}

	/* STAGE (2): Make the preference vector for the user */
	for (auto& rank : _Users[uIndex].ranks)
	{
		std::string title;
		double normalizedRank;
		std::vector<double> normalizedAttributes;
		if (rank.second[1] != NA)
		{
			title = rank.first;
			normalizedRank = rank.second[1];

			// Search for ranked movie in _Movies vector of Recommender System.
			auto iter = std::find_if(_Movies.begin(), _Movies.end(),
									 [&](const Movie &movie) { return movie.movieTitle == title; });
			if (iter != _Movies.end())  // Movie Found
			{
				int mIndex = std::distance(_Movies.begin(), iter);  // index of found movieTitle
				// Multiply the attributes vector of the movie by the normalized rank.
				std::transform(_Movies[mIndex].attributes.begin(),
							   _Movies[mIndex].attributes.end(),
							   std::back_inserter(normalizedAttributes),
							   [&] (double number) { return normalizedRank * number; });

				std::cout << title << " : ";
				std::for_each(normalizedAttributes.begin(), normalizedAttributes.end(),
							  [&](double number){ std::cout << number << " ";});
				std::cout << std::endl;

				// Updating the preference vector
				if ((_Users[uIndex].preferenceVector.size()) != (normalizedAttributes.size()))
				{
					_Users[uIndex].preferenceVector.resize(normalizedAttributes.size());
				}

				std::transform(_Users[uIndex].preferenceVector.begin(),
							   _Users[uIndex].preferenceVector.end(),
							   normalizedAttributes.begin(), _Users[uIndex].preferenceVector.begin(),
							   [&] (double number, double otherNumber) { return number + otherNumber; });
			}
		}
	}

	std::cout << "\npreferenceVector = ";
	std::for_each(_Users[uIndex].preferenceVector.begin(), _Users[uIndex].preferenceVector.end(),
				  [&](double number){ std::cout << number << " ";});
	std::cout << std::endl;

	/* STAGE (3): Calculate similarities between preference vector and unrated movies */
	std::pair<std::string, double> recommendedMovie("", 0.0);
	for (auto& rank : _Users[uIndex].ranks)
	{
		if (rank.second[0] == NA)
		{
			std::string title = rank.first;
			// Search for unrated movie in _Movies vector of Recommender System.
			auto iter = std::find_if(_Movies.begin(), _Movies.end(),
									 [&](const Movie &movie)	{ return movie.movieTitle == title;	});
			if (iter != _Movies.end())  // Movie Found
			{
				// index of found movieTitle
				int mIndex = std::distance(_Movies.begin(), iter);
				/* Apply the angleBetweenVectors on this movie and push back the result to
				 * the vector of similaritiesToPreferenceVector.
				 */
				const auto &pref = _Users[uIndex].preferenceVector;
				const auto &attr = _Movies[mIndex].attributes;
				double movieSimilarity = angleBetweenVectors(pref, attr);
				std::cout << title << " -> " << movieSimilarity << std::endl;
				if (movieSimilarity > recommendedMovie.second)
				{
					recommendedMovie.first = title;
					recommendedMovie.second = movieSimilarity;
				}
			}
		}
	}

	std::cout << "recommendedMovie : { " << recommendedMovie.first;
	std::cout << " } with movieSimilarity of : "<< recommendedMovie.second << std::endl;
	return recommendedMovie.first;
}

double RecommenderSystem::predictMovieScoreForUser(const std::string &movieName,
												   const std::string &userName, int k)
{
	/* Search for userName & movieName */
	auto userIter = std::find_if(_Users.begin(), _Users.end(),
						   [&](const User &user) { return user.userName == userName; });
	auto movieIter = std::find_if(_Movies.begin(), _Movies.end(),
							 [&](const Movie &movie) { return movie.movieTitle == movieName; });

	if (userIter == _Users.end() || movieIter == _Movies.end())  // Not Found
	{
		std::cerr << USER_NOT_FOUND << " or " << MOVIE_NOT_FOUND << std::endl;
		return NOT_FOUND;
	}

	int userIdx = std::distance(_Users.begin(), userIter);
	int movieIdx = std::distance(_Movies.begin(), movieIter);
	if (_Users[userIdx].ranks[movieName][0] != NA)
	{
		std::cerr << "ALREADY RATED BY USER\n"
		<< userName << " rated " << movieName << " with "
		<< _Users[userIdx].ranks[movieName][0] << std::endl;
		return _Users[userIdx].ranks[movieName][0];
	}

	/* Calculate similarities between movie attributes and rated movies */
	auto &similarities = _Movies[movieIdx].similaritiesToOtherMovies;
	for (auto& rank : _Users[userIdx].ranks)
	{
		if (rank.second[0] != NA)	// Movie is ranked by user
		{
			std::string title = rank.first;
			auto rankedMovieIter = std::find_if(_Movies.begin(), _Movies.end(),
					               [&](const Movie &movie)
					               { return movie.movieTitle == title; });
			int rankedMovieIdx = std::distance(_Movies.begin(), rankedMovieIter);


			const auto &pref = _Movies[rankedMovieIdx].attributes; // a rated movie attributes
			const auto &attr = _Movies[movieIdx].attributes;  // movieName attributes (not rated)
			double movieSimilarity = angleBetweenVectors(pref, attr);
//			std::cout << title << " -> " << movieSimilarity << std::endl;
			similarities.emplace_back(title, movieSimilarity, rank.second[0]);
		}
	}
	// Sort similaritiesToOtherMovies vector in descending order.
	std::sort(similarities.begin(), similarities.end(),
			[&](const auto &tuple1, const auto &tuple2)
			{ return std::get<1>(tuple1) > std::get<1>(tuple2); });
	/* Resize similaritiesToOtherMovies vector, so that it contains
	 * just the first k elements.
	 */
	similarities.resize(k);

	std::cout << "\nsimilaritiesToOtherMovies = \n";
	for (const auto& movie : similarities)
	{
		std::cout << std::get<0>(movie) << " which ranked with: ( " << std::get<2>(movie) << " ) "
		<< " similar to " << _Movies[movieIdx].movieTitle
		<< " by : " << std::get<1>(movie) << std::endl;
	}
	std::cout << std::endl;

	/* Calculate the expected rank */
	double numerator = 0;
	double denominator = 0;
	for (const auto& movie : similarities)
	{
		numerator += std::get<1>(movie) * std::get<2>(movie);
		denominator += std::get<1>(movie);
	}
	double expectedRank = numerator / denominator;
	std::cout << "expectedRank for " << movieName << " = " << expectedRank << std::endl;
	return expectedRank;
}

const std::string RecommenderSystem::recommendByCF(const std::string &userName, int k)
{
	/* Search for userName */
	auto userIter = std::find_if(_Users.begin(), _Users.end(),
						   [&](const User &user) { return user.userName == userName; });
	if (userIter == _Users.end())  // User Not Found
	{
		std::cerr << USER_NOT_FOUND << std::endl;
		return USER_NOT_FOUND;
	}

	int userIdx = std::distance(_Users.begin(), userIter);
	std::pair<std::string, double> recommendedMovie("", 0.0);
	for (const auto &movie : _Movies)
	{
		if (_Users[userIdx].ranks[movie.movieTitle][0] == NA)
		{
			double expectedRank = predictMovieScoreForUser(movie.movieTitle, userName, k);
			if (expectedRank > recommendedMovie.second)
			{
				recommendedMovie.first = movie.movieTitle;
				recommendedMovie.second = expectedRank;
			}
		}
	}

	std::cout << recommendedMovie.first << " is recommended for " << userName << std::endl;
	return recommendedMovie.first;
}

void RecommenderSystem::printDetails()
{
	std::cout << "\n|====== MOVIES ======|\n";
	for (const auto &movie : _Movies)
	{
		std::cout << "[ " << movie.movieTitle << " ] : ";
		for (auto attribute : movie.attributes)
		{
			std::cout << attribute << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "\n|====== RANKS ======|\n";
	for (const auto &user : _Users)
	{
		std::cout << "[ " << user.userName << " ] : \t";

		for (const auto& rank : user.ranks)
		{
			std::cout << "<" << rank.first << " : ";
			if (rank.second[0] != NA)
			{
				std::cout << rank.second[0];
			}
			else
			{
				std::cout << "NA";
			}
			std::cout << " , ";
			if (rank.second[1] != NA)
			{
				std::cout << rank.second[1];
			}
			else
			{
				std::cout << "NA";
			}
			std::cout << ">   ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

double RecommenderSystem::vectorNorm(const std::vector<double> &vec)
{
	return sqrt(std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0));
}

double RecommenderSystem::angleBetweenVectors(const std::vector<double> &vec1,
											  const std::vector<double> &vec2)
{
	return (std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0.0))
			/ (vectorNorm(vec1) * vectorNorm(vec2));
}


int main()
{
	auto t1 = std::chrono::high_resolution_clock::now();

	RecommenderSystem rec;

	rec.loadData(MOVIES_BIG, RANKS_BIG);
//	rec.loadData(MOVIES_SMALL, RANKS_SMALL);
//	rec.loadData(MOVIES_FEATURES, RANKS_MATRIX);
//	rec.printDetails();
//	rec.recommendByContent("Muaz");
//	rec.recommendByContent("Sofia");
	rec.recommendByContent("Sophia");
//	rec.predictMovieScoreForUser("Titanic", "Nicole", 2);
//	rec.predictMovieScoreForUser("Twilight", "Nicole", 2);
	rec.recommendByCF("Sophia", 2);
//	rec.printDetails();


	auto t2 = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
	std::cout << "time: ( " << (double) time / 1000.0 << " ) seconds" << std::endl;

	return 0;
}
