-- phpMyAdmin SQL Dump
-- version 4.8.3
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1:3306
-- Généré le :  Dim 16 déc. 2018 à 19:21
-- Version du serveur :  5.7.23
-- Version de PHP :  7.2.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `dbbenchmark`
--

-- --------------------------------------------------------

--
-- Structure de la table `benchmarks`
--

DROP TABLE IF EXISTS `benchmarks`;
CREATE TABLE IF NOT EXISTS `benchmarks` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `userID` int(11) NOT NULL,
  `Date` varchar(255) NOT NULL,
  `databaseType` varchar(255) NOT NULL,
  `hostname` varchar(255) NOT NULL,
  `port` int(11) NOT NULL,
  `databaseName` varchar(255) NOT NULL,
  `user` varchar(255) NOT NULL,
  `requestNumber` int(11) NOT NULL,
  `pingCompensation` smallint(6) NOT NULL,
  `customScript` smallint(6) NOT NULL,
  `multiThreads` smallint(6) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=19 DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `benchmarks`
--

INSERT INTO `benchmarks` (`ID`, `userID`, `Date`, `databaseType`, `hostname`, `port`, `databaseName`, `user`, `requestNumber`, `pingCompensation`, `customScript`, `multiThreads`) VALUES
(1, 1, '2018-12-07 00:00:00', 'MySql', 'localhost', 1, 'paul', 'root', 10, 0, 0, 0),
(2, 1, '2018-12-07 00:00:00', 'MySql', 'localhost', 2, 'paul', 'root', 12, 0, 1, 0),
(3, 1, '2018-12-13 16:42:42', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 6, 0, 0, 0),
(4, 1, '2018-12-13 16:48:41', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 10, 0, 0, 0),
(5, 1, '2018-12-13 16:53:1', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 2, 0, 0, 0),
(6, 1, '2018-12-13 16:55:19', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 3, 0, 0, 0),
(7, 1, '2018-12-13 17:0:21', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 6, 0, 0, 0),
(12, 1, '2018-12-13 17:12:50', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 13, 0, 0, 0),
(13, 1, '2018-12-14 0:12:53', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 9, 0, 0, 0),
(14, 1, '2018-12-14 0:16:41', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 3, 0, 0, 0),
(15, 1, '2018-12-14 0:20:15', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 11, 0, 0, 0),
(16, 1, '2018-12-14 0:22:41', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 8, 0, 0, 0),
(17, 1, '2018-12-14 0:24:22', 'MySql', 'www.db4free.net', 3306, 'paulbenchmark', 'paulbenchmark', 6, 0, 0, 0);

-- --------------------------------------------------------

--
-- Structure de la table `results`
--

DROP TABLE IF EXISTS `results`;
CREATE TABLE IF NOT EXISTS `results` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `benchmark_id` int(11) NOT NULL,
  `score` int(11) NOT NULL,
  `results` json NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `results`
--

INSERT INTO `results` (`ID`, `benchmark_id`, `score`, `results`) VALUES
(1, 12, 67, '{\"name\": \"Awesome 4K\", \"resolutions\": [{\"width\": 1280, \"height\": 720}, {\"width\": 1920, \"height\": 1080}, {\"width\": 3840, \"height\": 2160}]}'),
(2, 14, 49, '{\"name\": \"Awesome 4K\", \"resolutions\": [{\"width\": 306.6195, \"height\": 23.549}, {\"width\": 614.1457, \"height\": 23.0096}, {\"width\": 119.192, \"height\": 21.5921}]}'),
(3, 17, 47, '{\"datas\": [{\"read\": 24.4854, \"write\": 94.6138}, {\"read\": 23.2924, \"write\": 109.0349}, {\"read\": 24.0099, \"write\": 1025.1396}, {\"read\": 24.2654, \"write\": 306.9888}, {\"read\": 23.8919, \"write\": 307.1779}, {\"read\": 24.2851, \"write\": 204.8965}]}');

-- --------------------------------------------------------

--
-- Structure de la table `users`
--

DROP TABLE IF EXISTS `users`;
CREATE TABLE IF NOT EXISTS `users` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `users`
--

INSERT INTO `users` (`ID`, `name`, `password`) VALUES
(1, 'paul', 'test');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
