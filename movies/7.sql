---all movies released in 2010 and their ratings DESC, alph when tied, no null ratings
SELECT movies.title, ratings.rating
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE year = 2010 AND rating IS NOT NULL
ORDER BY rating DESC, title;

