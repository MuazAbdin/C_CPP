#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

/**
 * @file RecommenderSystem.h
 * @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
 * @ID 300575297
 * @date 20 May 2020
 *
 * @brief DESCRIPTION:
 * 		  A programme for recommending movies for uesrs according to their rantings.
 */

#include <string>
#include <vector>
#include <map>
#include <unordered_map>


/**
 * @brief Recommender System Class.
 */
class RecommenderSystem
{
private:
	std::vector<std::string> _usersNames;  // in Ranks File order
	std::vector<std::string> _moviesTitles;  // in Ranks File order
	std::vector<std::vector<double>> _allRatings;  // Matrix of all ratings (Ranks File)
	std::unordered_map<std::string, std::vector<double>> _moviesAttributes;  // (Attributes File)

	/**
	 * @brief Load movies attributes file.
	 * @param moviesAttributesFilePath
	 * @return false if fails, true if succeeds.
	 */
	bool _loadMoviesAttributes(const std::string &moviesAttributesFilePath);

	/**
	 * @brief Load users ratings file
	 * @param userRanksFilePath
	 * @return false if fails, true if succeeds.
	 */
	bool _loadUsersRatings(const std::string &userRanksFilePath);

	/**
	 * @brief gets the index of a given entry.
	 * @param entry: entry to search for.
	 * @param entriesVec: vector to search in.
	 * @return the index of a given entry if founded, else -1.
	 */
	int _getEntryIdx(const std::string &entry, const std::vector<std::string> &entriesVec) const;

	/**
	 * @brief normalize the user ratings for movies.
	 * @param vec: vector to put the normalized ranks in.
	 * @param userIdx: index of user in _userNames.
	 */
	void _normalizeRanks(std::vector<double> &vec, int userIdx);

	/**
	 * @brief Make Preference Vector for a user.
	 * @param rankVec: vector of normalized ranks.
	 * @param prefVec: vector to put preferences in .
	 */
	void _makePreferenceVector(std::vector<double> &rankVec, std::vector<double> &prefVec);

	/**
	 * @brief Calculates the angle between vectors.
	 * @param vec1: first vector.
	 * @param vec2: second vector.
	 * @return the angle between the given vectors.
	 */
	double _calculateSimilarity(const std::vector<double> &vec1, const std::vector<double> &vec2);


public:
	/**
	 * @brief Loading data from input files to RecommenderSystem
	 * @param moviesAttributesFilePath: path of file.
	 * @param userRanksFilePath: path of file.
	 * @return 0 if succeeds, otherwise -1.
	 */
	int loadData(const std::string &moviesAttributesFilePath,
			     const std::string &userRanksFilePath);

	/**
	 * @brief Recommend a movies according to its content.
	 * @param userName
	 * @return the name of the recommended movie.
	 */
	const std::string recommendByContent(const std::string &userName);

	/**
	 * @brief Predict a rank for unrated movie according to other user ratings.
	 * @param movieName
	 * @param userName
	 * @param k: natural number represents the most similar movies.
	 * @return the predicted rank.
	 */
	double predictMovieScoreForUser(const std::string &movieName,
									const std::string &userName, int k);

	/**
	 * @brief Recommend a movie by collaborative filtering.
	 * @param userName
	 * @param k: natural number represents the most similar movies.
	 * @return the name of the recommended movie.
	 */
	const std::string recommendByCF(const std::string &userName, int k);

};

#endif //EX5_RECOMMENDERSYSTEM_H
