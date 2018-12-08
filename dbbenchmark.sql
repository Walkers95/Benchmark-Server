-- phpMyAdmin SQL Dump
-- version 4.8.3
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1:3306
-- Généré le :  sam. 08 déc. 2018 à 12:04
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
  `Date` date NOT NULL,
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
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `benchmarks`
--

INSERT INTO `benchmarks` (`ID`, `userID`, `Date`, `databaseType`, `hostname`, `port`, `databaseName`, `user`, `requestNumber`, `pingCompensation`, `customScript`, `multiThreads`) VALUES
(1, 1, '2018-12-07', 'MySql', 'localhost', 1, 'paul', 'root', 10, 0, 0, 0),
(2, 1, '2018-12-07', 'MySql', 'localhost', 2, 'paul', 'root', 12, 0, 1, 0);

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
