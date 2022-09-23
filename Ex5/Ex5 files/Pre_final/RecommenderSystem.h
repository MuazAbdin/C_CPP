#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

#include <string>
#include <vector>
#include <map>




/**
 * @brief
 */
class RecommenderSystem
{
private:
	/**
	 * @brief Struct holds movie's similarities and attributes.
	 */
	struct Movie
	{
		std::string movieTitle;
		std::vector<double> attributes;
		std::vector<std::tuple<std::string, double, int>> similaritiesToOtherMovies;
		bool operator<(const Movie &rhs) const
		{
			return movieTitle.compare(rhs.movieTitle) < 0;
		}
	};

	/**
	 * @brief Struct holds user's ranks and preference vector.
	 */
	struct User
	{
		std::string userName;
		std::map<const std::string, double[2]> ranks;
		std::vector<double> preferenceVector;
		bool operator<(const User &rhs) const
		{
			return userName.compare(rhs.userName) < 0;
		}
	};

	std::vector<Movie> _Movies;
	std::vector<User> _Users;

	double vectorNorm(const std::vector<double> &vec);
	double angleBetweenVectors(const std::vector<double> &vec1, const std::vector<double> &vec2);



public:
	/**
	 * @brief
	 * @param moviesAttributesFilePath
	 * @param userRanksFilePath
	 * @return
	 */
	int loadData(const std::string &moviesAttributesFilePath,
				  const std::string &userRanksFilePath);

	/**
	 * @brief
	 * @param userName
	 * @return
	 */
	const std::string recommendByContent(const std::string &userName);

	/**
	 * @brief
	 * @param movieName
	 * @param userName
	 * @param k
	 * @return
	 */
	double predictMovieScoreForUser(const std::string &movieName,
								 const std::string &userName, int k);

	/**
	 * @brief
	 * @param userName
	 * @param k
	 * @return
	 */
	const std::string recommendByCF(const std::string &userName, int k);


	void printDetails();
};

#endif //EX5_RECOMMENDERSYSTEM_H
