/**
 * @file RecommenderSystem.cpp
 * @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
 * @ID 300575297
 * @date 20 May 2020
 *
 * @brief DESCRIPTION:
 * 		  A programme for recommending movies for uesrs according to their rantings.
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

// ------------------------------ macros & constants --------------------------------

#define FILE_NOT_OPENED "Unable to open file "
#define USER_NOT_FOUND "USER NOT FOUND"

#define NA -25.0  // NA = Not rated by user
#define NOT_FOUND -1
#define LOAD_FAIL -1
#define LOAD_SUCCESS 0


// ------------------------------ functions implementation ---------------------------

/**
 * @brief Load movies attributes file.
 * @param moviesAttributesFilePath
 * @return false if fails, true if succeeds.
 */
bool RecommenderSystem::_loadMoviesAttributes (const std::string &moviesAttributesFilePath)
{
	/* Loading the movies attributes file */
	std::ifstream moviesAttributesFile;
	moviesAttributesFile.open(moviesAttributesFilePath, std::ios::in);
	if (!moviesAttributesFile.is_open())
	{
		std::cerr << FILE_NOT_OPENED << moviesAttributesFilePath << std::endl;
		return false;
	}
	std::string movieDetails;
	while (std::getline(moviesAttributesFile, movieDetails))
	{
		std::istringstream issM(movieDetails);
		std::string movieTitle;
		std::vector<double> singleMovieAttributes;
		issM >> movieTitle;
		for (int i; issM >> i; )
		{
			singleMovieAttributes.push_back(i);
		}
		_moviesAttributes[movieTitle] = singleMovieAttributes;
	}
	moviesAttributesFile.close();
	return true;
}

/**
 * @brief Load users ratings file
 * @param userRanksFilePath
 * @return false if fails, true if succeeds.
 */
bool RecommenderSystem::_loadUsersRatings(const std::string &userRanksFilePath)
{
	std::ifstream userRanksFile;
	userRanksFile.open(userRanksFilePath, std::ios::in);
	if (!userRanksFile.is_open())
	{
		std::cerr << FILE_NOT_OPENED << userRanksFilePath << std::endl;
		return false;
	}
	std::string userRanks;
	std::getline(userRanksFile, userRanks);
	std::istringstream issU1(userRanks);
	for (std::string title; issU1 >> title; )
	{
		_moviesTitles.push_back(title);
	}
	while (std::getline(userRanksFile, userRanks))
	{
		std::istringstream issU2(userRanks);
		std::string userName;
		std::vector<double> userSingleMovieRate;
		issU2 >> userName;
		_usersNames.push_back(userName);
		for (size_t i = 0; i < _moviesTitles.size(); ++i)
		{
			int j;
			issU2 >> j;
			if (issU2.eof())
			{
				break;
			}
			if (issU2.fail())
			{
				userSingleMovieRate.push_back(NA);
				issU2.clear();
				issU2.ignore(100, ' ');
			}
			else
			{
				userSingleMovieRate.push_back(j);
			}
		}
		_allRatings.emplace_back(userSingleMovieRate);
	}
	userRanksFile.close();
	return true;
}

/**
 * @brief Calculates the angle between vectors.
 * @param vec1: first vector.
 * @param vec2: second vector.
 * @return the angle between the given vectors.
 */
double RecommenderSystem::_calculateSimilarity(const std::vector<double> &vec1,
											   const std::vector<double> &vec2)
{
	double normVec1 = sqrt(std::inner_product(vec1.begin(), vec1.end(), vec1.begin(), 0.0));
	double normVec2 = sqrt(std::inner_product(vec2.begin(), vec2.end(), vec2.begin(), 0.0));

	return (std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0.0))
		   / (normVec1 * normVec2);
}

/**
 * @brief Loading data from input files to RecommenderSystem
 * @param moviesAttributesFilePath: path of file.
 * @param userRanksFilePath: path of file.
 * @return 0 if succeeds, otherwise -1.
 */
int RecommenderSystem::loadData(const std::string &moviesAttributesFilePath,
								const std::string &userRanksFilePath)
{
	if (!(_loadMoviesAttributes(moviesAttributesFilePath) &&
	    _loadUsersRatings(userRanksFilePath)))
	{
		return LOAD_FAIL;
	}

	return LOAD_SUCCESS;
}

/**
 * @brief gets the index of a given entry.
 * @param entry: entry to search for.
 * @param entriesVec: vector to search in.
 * @return the index of a given entry if founded, else -1.
 */
int RecommenderSystem::_getEntryIdx(const std::string &entry,
								    const std::vector<std::string> &entriesVec) const
{
	auto it = std::find(entriesVec.begin(), entriesVec.end(), entry);
	if (it == entriesVec.end())  // entry not found
	{
		return NOT_FOUND;
	}
	int entryIndex = std::distance(entriesVec.begin(), it);  // index of found entry
	return entryIndex;
}

/**
 * @brief normalize the user ratings for movies.
 * @param vec: vector to put the normalized ranks in.
 * @param userIdx: index of user in _userNames.
 */
void RecommenderSystem::_normalizeRanks(std::vector<double> &vec, int userIdx)
{
	double ranksSum = 0;
	int numRated = 0;
	size_t matCols = _moviesTitles.size();
	for (size_t i = 0; i < matCols; ++i)
	{
		double rank = _allRatings[userIdx][i];
		if (rank != NA)
		{
			ranksSum += _allRatings[userIdx][i];
			numRated++;
		}
	}
	double ranksAvg = ranksSum / numRated;
	for (size_t i = 0; i < matCols; ++i)
	{
		double rank = _allRatings[userIdx][i];
		vec.emplace_back((rank != NA) ? rank - ranksAvg : NA);
	}
}

/**
 * @brief Make Preference Vector for a user.
 * @param rankVec: vector of normalized ranks.
 * @param prefVec: vector to put preferences in .
 */
void RecommenderSystem::_makePreferenceVector(std::vector<double> &rankVec,
											  std::vector<double> &prefVec)
{
	size_t matCols = _moviesTitles.size();
	for (size_t idx = 0; idx < matCols; ++idx)  // matCols = normalizedRanks.size() - 1
	{
		std::vector<double> normalizedAttributes;
		if (rankVec[idx] != NA)
		{
			// Multiply the attributes vector of the movie by the normalized rank.
			std::string title = _moviesTitles[idx];
			std::transform(_moviesAttributes[title].begin(),
						   _moviesAttributes[title].end(),
						   std::back_inserter(normalizedAttributes),
						   [&] (double number)
						   { return rankVec[idx] * number; });
			// Updating the preference vector
			std::transform(prefVec.begin(), prefVec.end(),
						   normalizedAttributes.begin(), prefVec.begin(),
						   [&] (double number, double otherNumber)
						   { return number + otherNumber; });
		}
	}
}

/**
 * @brief Recommend a movies according to its content.
 * @param userName
 * @return the name of the recommended movie.
 */
const std::string RecommenderSystem::recommendByContent(const std::string &userName)
{
	/* Search for userName */
	int userNameIdx = _getEntryIdx(userName, _usersNames);
	if (userNameIdx == NOT_FOUND)
	{
		std::cerr << USER_NOT_FOUND << std::endl;
		return USER_NOT_FOUND;
	}

	/* STAGE (1): Calculate the normalized ranks for the user */
	std::vector<double> normalizedRanks;
	_normalizeRanks(normalizedRanks, userNameIdx);

	/* STAGE (2): Make the preference vector for the user */
	int numAttributes = _moviesAttributes[_moviesTitles[0]].size();
	std::vector<double> preferenceVector(numAttributes);
	_makePreferenceVector(normalizedRanks, preferenceVector);

	/* STAGE (3): Calculate similarities between preference vector and unrated movies */
	std::pair<std::string, double> recommendedMovie("", -1.0);
	size_t matCols = _moviesTitles.size();
	for (size_t idx = 0; idx < matCols; ++idx)  // matCols = number of ranks = movies
	{
		if (_allRatings[userNameIdx][idx] == NA)  // Movie is not rated
		{
			std::string title = _moviesTitles[idx];
			double movieSimilarity = _calculateSimilarity(preferenceVector,
					_moviesAttributes[title]);
			if (movieSimilarity > recommendedMovie.second)
			{
				recommendedMovie.first = title;
				recommendedMovie.second = movieSimilarity;
			}
		}
	}
	return recommendedMovie.first;
}

/**
 * @brief Predict a rank for unrated movie according to other user ratings.
 * @param movieName
 * @param userName
 * @param k: natural number represents the most similar movies.
 * @return the predicted rank.
 */
double RecommenderSystem::predictMovieScoreForUser(const std::string &movieName,
												   const std::string &userName, int k)
{
	/* Search for userName & movieName */
	int userNameIdx = _getEntryIdx(userName, _usersNames);
	int movieTitleIdx = _getEntryIdx(movieName, _moviesTitles);
	if (userNameIdx == NOT_FOUND || movieTitleIdx == NOT_FOUND)
	{
		return NOT_FOUND;
	}

	/* Calculate similarities between movie attributes and rated movies */
	std::vector<std::pair<int, double>> similarities;   /* <rank, similarity> */
	for (size_t idx = 0; idx < _moviesTitles.size(); ++idx)  // matCols = number of ranks = movies
	{
		int movieRank = _allRatings[userNameIdx][idx];
		if (movieRank != NA)  // Movie is ranked by user
		{
			std::string title = _moviesTitles[idx];
			double movieSimilarity = _calculateSimilarity(_moviesAttributes[title],
														  _moviesAttributes[movieName]);
			similarities.emplace_back(movieRank, movieSimilarity);
		}
	}
	/* Sort similarities vector in descending order and resize it, so that it contains
	 * just the first k elements.
	 */
	std::sort(similarities.begin(), similarities.end(),
			[&](const auto &pair1, const auto &pair2)
			{ return pair1.second > pair2.second; });
	similarities.resize(k);

	/* Calculate the expected rank */
	double numerator = 0;
	double denominator = 0;
	for (const auto& movie : similarities)
	{
		numerator += movie.first * movie.second;
		denominator += movie.second;
	}
	double expectedRank = numerator / denominator;
	return expectedRank;
}

/**
 * @brief Recommend a movie by collaborative filtering.
 * @param userName
 * @param k: natural number represents the most similar movies.
 * @return the name of the recommended movie.
 */
const std::string RecommenderSystem::recommendByCF(const std::string &userName, int k)
{
	/* Search for userName */
	int userNameIdx = _getEntryIdx(userName, _usersNames);
	if (userNameIdx == NOT_FOUND)
	{
		return USER_NOT_FOUND;
	}
	std::pair<std::string, double> recommendedMovie("", -1.0);
	for (size_t idx = 0; idx < _moviesTitles.size(); ++idx)
	{
		if (_allRatings[userNameIdx][idx] == NA)  // Movie is not rated
		{
			std::string title = _moviesTitles[idx];
			double expectedRank = predictMovieScoreForUser(title, userName, k);
			if (expectedRank > recommendedMovie.second)
			{
				recommendedMovie.first = title;
				recommendedMovie.second = expectedRank;
			}
		}
	}
	return recommendedMovie.first;
}
