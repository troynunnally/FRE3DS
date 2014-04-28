-- phpMyAdmin SQL Dump
-- version 3.5.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Apr 24, 2014 at 10:43 PM
-- Server version: 5.5.24-log
-- PHP Version: 5.4.3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `navsec`
--
CREATE DATABASE `navsec` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `navsec`;

-- --------------------------------------------------------

--
-- Table structure for table `active_sessions`
--

CREATE TABLE IF NOT EXISTS `active_sessions` (
  `aid` int(11) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `sid` int(11) NOT NULL,
  `steps` int(11) NOT NULL,
  PRIMARY KEY (`aid`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `active_sessions`
--

INSERT INTO `active_sessions` (`aid`, `uid`, `sid`, `steps`) VALUES
(1, 1, 1, 156);

-- --------------------------------------------------------

--
-- Table structure for table `interactions`
--

CREATE TABLE IF NOT EXISTS `interactions` (
  `iid` int(11) NOT NULL AUTO_INCREMENT,
  `table_column` varchar(255) NOT NULL,
  `description` varchar(255) NOT NULL,
  PRIMARY KEY (`iid`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=25 ;

--
-- Dumping data for table `interactions`
--

INSERT INTO `interactions` (`iid`, `table_column`, `description`) VALUES
(1, 'left_plane', 'Add Left Plane'),
(2, 'right_plane', 'Add Right Plane'),
(3, 'camera_home', 'Camera Home'),
(4, 'rotate_15_x', 'Rotate 15 degrees to left'),
(5, 'rotate_30_x', 'Rotate 30 degrees to left'),
(6, 'rotate_45_x', 'Rotate 45 degrees to left'),
(7, 'rotate_60_x', 'Rotate 60 degrees to left'),
(8, 'rotate_75_x', 'Rotate 75 degrees to left'),
(9, 'rotate_90_x', 'Rotate 90 degrees to left'),
(10, 'zoom_out_10', 'Zoom Out by 10'),
(11, 'zoom_out_20', 'Zoom Out by 20'),
(12, 'zoom_out_30', 'Zoom Out by 30'),
(13, 'zoom_out_40', 'Zoom Out by 40'),
(14, 'zoom_out_50', 'Zoom Out by 50 or more'),
(15, 'zoom_in_10', 'Zoom In by 10'),
(16, 'zoom_in_20', 'Zoom In by 20'),
(17, 'zoom_in_30', 'Zoom In by 30'),
(18, 'zoom_in_40', 'Zoom In by 40'),
(19, 'zoom_in_50', 'Zoom In by 50 or more'),
(20, 'link_color', 'Add Green Link Color'),
(21, 'pckt_size_y', 'Average Packet Size Y Axis'),
(22, 'total_num_pckt_z', 'Total Number Packets -  Z Axis'),
(23, 'pckt_size_z', 'Average Packet Size Z Axis'),
(24, 'total_num_pckt_y', 'Total Number Packets -  Y Axis');

-- --------------------------------------------------------

--
-- Table structure for table `session_vectors`
--

CREATE TABLE IF NOT EXISTS `session_vectors` (
  `vid` int(11) NOT NULL AUTO_INCREMENT,
  `sid` int(11) NOT NULL,
  `left_plane` int(11) NOT NULL DEFAULT '0',
  `right_plane` int(11) NOT NULL DEFAULT '0',
  `link_color` int(11) NOT NULL,
  `pckt_size_y` int(11) NOT NULL DEFAULT '0',
  `pckt_size_z` int(11) NOT NULL DEFAULT '0',
  `total_num_pckt_y` int(11) NOT NULL DEFAULT '0',
  `total_num_pckt_z` int(11) NOT NULL DEFAULT '0',
  `s_port_y` int(11) NOT NULL DEFAULT '0',
  `s_port_z` int(11) NOT NULL DEFAULT '0',
  `d_port_y` int(11) NOT NULL DEFAULT '0',
  `d_port_z` int(11) NOT NULL DEFAULT '0',
  `s_ip_y` int(11) NOT NULL DEFAULT '0',
  `s_ip_z` int(11) NOT NULL DEFAULT '0',
  `d_ip_y` int(11) NOT NULL DEFAULT '0',
  `d_ip_z` int(11) NOT NULL DEFAULT '0',
  `transport_protocol` int(11) NOT NULL DEFAULT '0',
  `network_protocol` int(11) NOT NULL DEFAULT '0',
  `ip_flag` int(11) NOT NULL DEFAULT '0',
  `time_y` int(11) NOT NULL DEFAULT '0',
  `time_z` int(11) NOT NULL DEFAULT '0',
  `zoom_out_10` int(11) NOT NULL DEFAULT '0',
  `zoom_out_20` int(11) NOT NULL DEFAULT '0',
  `zoom_out_30` int(11) NOT NULL DEFAULT '0',
  `zoom_out_40` int(11) NOT NULL DEFAULT '0',
  `zoom_out_50` int(11) NOT NULL DEFAULT '0',
  `zoom_in_10` int(11) NOT NULL DEFAULT '0',
  `zoom_in_20` int(11) NOT NULL DEFAULT '0',
  `zoom_in_30` int(11) NOT NULL DEFAULT '0',
  `zoom_in_40` int(11) NOT NULL DEFAULT '0',
  `zoom_in_50` int(11) NOT NULL DEFAULT '0',
  `rotate_15_x` int(11) NOT NULL DEFAULT '0',
  `rotate_30_x` int(11) NOT NULL DEFAULT '0',
  `rotate_45_x` int(11) NOT NULL DEFAULT '0',
  `rotate_60_x` int(11) NOT NULL DEFAULT '0',
  `rotate_75_x` int(11) NOT NULL DEFAULT '0',
  `rotate_90_x` int(11) NOT NULL DEFAULT '0',
  `rotate_15_y` int(11) NOT NULL DEFAULT '0',
  `rotate_30_y` int(11) NOT NULL DEFAULT '0',
  `rotate_45_y` int(11) NOT NULL DEFAULT '0',
  `rotate_60_y` int(11) NOT NULL DEFAULT '0',
  `rotate_75_y` int(11) NOT NULL DEFAULT '0',
  `rotate_90_y` int(11) NOT NULL DEFAULT '0',
  `translate_x` int(11) NOT NULL DEFAULT '0',
  `translate_y` int(11) NOT NULL DEFAULT '0',
  `translate_z` int(11) NOT NULL DEFAULT '0',
  `clear` int(11) NOT NULL DEFAULT '0',
  `camera_home` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`vid`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=11 ;

--
-- Dumping data for table `session_vectors`
--

INSERT INTO `session_vectors` (`vid`, `sid`, `left_plane`, `right_plane`, `link_color`, `pckt_size_y`, `pckt_size_z`, `total_num_pckt_y`, `total_num_pckt_z`, `s_port_y`, `s_port_z`, `d_port_y`, `d_port_z`, `s_ip_y`, `s_ip_z`, `d_ip_y`, `d_ip_z`, `transport_protocol`, `network_protocol`, `ip_flag`, `time_y`, `time_z`, `zoom_out_10`, `zoom_out_20`, `zoom_out_30`, `zoom_out_40`, `zoom_out_50`, `zoom_in_10`, `zoom_in_20`, `zoom_in_30`, `zoom_in_40`, `zoom_in_50`, `rotate_15_x`, `rotate_30_x`, `rotate_45_x`, `rotate_60_x`, `rotate_75_x`, `rotate_90_x`, `rotate_15_y`, `rotate_30_y`, `rotate_45_y`, `rotate_60_y`, `rotate_75_y`, `rotate_90_y`, `translate_x`, `translate_y`, `translate_z`, `clear`, `camera_home`) VALUES
(1, 1, 7, 0, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 3, 3, 2, 10, 0, 4, 2, 2, 7, 67, 3, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1),
(2, 2, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(3, 3, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(4, 4, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 2, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(5, 5, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(6, 6, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 4, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(7, 7, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(8, 8, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9, 9, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 2, 7, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(10, 10, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `sessions`
--

CREATE TABLE IF NOT EXISTS `sessions` (
  `sid` int(11) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `commands` text CHARACTER SET utf8 NOT NULL,
  PRIMARY KEY (`sid`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=11 ;

--
-- Dumping data for table `sessions`
--

INSERT INTO `sessions` (`sid`, `uid`, `commands`) VALUES
(1, 1, '["Rotate 45 degrees to left","Rotate 30 degrees to left","Rotate 45 degrees to left","Zoom Out by 50 or more","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom Out by 50 or more","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 45 degrees to left","Rotate 15 degrees to left","Zoom Out by 50 or more","Rotate 15 degrees to left","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Zoom Out by 10","Zoom Out by 10","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 30 degrees to left","Zoom Out by 20","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom Out by 10","Rotate 15 degrees to left","Zoom Out by 10","Add Left Plane","Rotate 15 degrees to left","Zoom Out by 10","Zoom Out by 10","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom Out by 50 or more","Rotate 15 degrees to left","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Zoom In by 30","Zoom In by 20","Rotate 15 degrees to left","Zoom Out by 20","Zoom In by 20","Rotate 15 degrees to left","Zoom Out by 10","Zoom Out by 10","Zoom Out by 50 or more","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Zoom In by 50 or more","Zoom In by 40","Zoom Out by 40","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Zoom Out by 10","Zoom Out by 10","Zoom Out by 10","Rotate 15 degrees to left","Zoom Out by 50 or more","Rotate 15 degrees to left","Zoom Out by 30","Rotate 15 degrees to left","Zoom Out by 40","Rotate 15 degrees to left","Zoom In by 40","Zoom Out by 30","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom In by 20","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom In by 20","Rotate 15 degrees to left","Rotate 60 degrees to left","Rotate 15 degrees to left","Zoom Out by 30","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom Out by 50 or more","Rotate 15 degrees to left","Rotate 60 degrees to left","Zoom In by 30","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Camera Home","Zoom Out by 50 or more","Rotate 15 degrees to left","Zoom Out by 50 or more","Rotate 75 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Zoom Out by 50 or more","Rotate 45 degrees to left","Rotate 15 degrees to left","Zoom Out by 10","Zoom Out by 10","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Rotate 30 degrees to left","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom Out by 10","Zoom Out by 20","Zoom Out by 10","Zoom Out by 10","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Rotate 15 degrees to left"]'),
(2, 3, '["Rotate 15 degrees to left","Rotate 15 degrees to left","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Zoom Out by 40","Zoom In by 50 or more","Rotate 30 degrees to left"]'),
(3, 2, '["Zoom Out by 20","Rotate 15 degrees to left","Rotate 15 degrees to left","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Zoom Out by 30","Rotate 60 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left"]'),
(4, 2, '["Zoom Out by 30","Rotate 15 degrees to left","Rotate 30 degrees to left","Zoom In by 50 or more","Rotate 30 degrees to left","Zoom Out by 50 or more","Zoom Out by 40","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Zoom Out by 20","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Zoom In by 20","Rotate 15 degrees to left"]'),
(5, 2, '["Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Zoom In by 40","Rotate 15 degrees to left","Zoom Out by 50 or more","Zoom In by 30","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left"]'),
(6, 3, '["Zoom In by 50 or more","Rotate 15 degrees to left","Rotate 45 degrees to left","Zoom Out by 50 or more","Rotate 15 degrees to left","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Add Left Plane","Rotate 30 degrees to left","Zoom In by 40","Rotate 15 degrees to left","Zoom Out by 10","Rotate 60 degrees to left","Zoom In by 30","Rotate 15 degrees to left"]'),
(7, 3, '["Zoom Out by 50 or more","Rotate 15 degrees to left","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Zoom In by 50 or more","Rotate 15 degrees to left","Zoom In by 40","Rotate 15 degrees to left","Zoom Out by 50 or more"]'),
(8, 3, '["Rotate 15 degrees to left","Zoom Out by 20","Rotate 15 degrees to left","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Zoom In by 50 or more"]'),
(9, 4, '["Zoom Out by 20","Rotate 15 degrees to left","Rotate 45 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom Out by 50 or more","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom In by 40","Rotate 15 degrees to left"]'),
(10, 4, '["Zoom Out by 20","Rotate 15 degrees to left","Rotate 15 degrees to left","Zoom In by 50 or more","Rotate 15 degrees to left","Zoom Out by 50 or more","Rotate 30 degrees to left","Zoom Out by 30","Add Left Plane","Add Green Link Color","Average Packet Size Y Axis","Total Number Packets -  Z Axis","Rotate 15 degrees to left","Zoom In by 40","Rotate 15 degrees to left"]');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `password` char(40) NOT NULL,
  `registration_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`uid`, `name`, `email`, `password`, `registration_time`) VALUES
(1, 'Troy Nunnally', 'troy.nunnally@gatech.edu', 'bc9189406be84ec297464a514221406d', '2013-06-04 17:16:48'),
(2, 'Raheem Beyah', 'troynunnally@gmail.com', 'bc9189406be84ec297464a514221406d', '2013-04-05 10:35:49'),
(3, 'Selcuk Uluagac', 'troynunnally@gmail.com', 'bc9189406be84ec297464a514221406d', '2013-04-05 10:44:39'),
(4, 'Kulsoom Abdullah', 'troynunnally@gmail.com', 'bc9189406be84ec297464a514221406d', '2013-04-05 10:46:49');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
