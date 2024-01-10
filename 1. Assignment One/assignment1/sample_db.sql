DROP TABLE IF EXISTS `works`;
DROP TABLE IF EXISTS `emp`;
DROP TABLE IF EXISTS `dept`;

CREATE TABLE `dept` (
  `did` int(11) NOT NULL,
  `dname` varchar(40) NOT NULL,
  `budget` double NOT NULL,
  `managerid` int(11) NOT NULL,
  PRIMARY KEY (`did`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- Dumping data for table `dept`
--


INSERT INTO `dept` VALUES (3,'Software',1439119,110183),(4,'Human Resources',1174402,110303),(5,'Research',1544653,110511),(6,'Business Development',0,110303),(8,'Hardware',4291505,111692),(9,'Customer Service',1308515,111692);

--
-- Table structure for table `emp`
--


CREATE TABLE `emp` (
  `eid` int(11) NOT NULL,
  `ename` varchar(32) NOT NULL,
  `age` int(11) NOT NULL,
  `salary` double NOT NULL,
  PRIMARY KEY (`eid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- Dumping data for table `emp`
--


INSERT INTO `emp` VALUES (10042,'Magy Stamatiou',60,81662),(10059,'Alejandro McAlpine',63,71218),(10073,'Shir McClurg',62,56473),(110183,'Shirish Ossenbruggen',63,48291),(110303,'Krassimir Wegerle',60,1374402),(110511,'DeForest Hagimont',59,48626),(111400,'Arie Staelin',57,72446),(111692,'Tonny Butterworth',62,40000),(403353,'Shigehito Kropatsch',52,59783),(499868,'Shaz Bierbaum',60,61802),(499950,'Weidon Gente',52,54788),(499998,'Patricia Breugel',60,40000), (12000,'Alex Dalas',55,54000), (12010,'Sergio Ravarez',55,64000), (12020,'Antonio Lavante',34,36000), (12030,'Tonny Conner',25,44000), (12100,'Gladys Cooper',32,77000), (12110,'Rodney Ferreri',37,99000);


--
-- Table structure for table `works`
--

CREATE TABLE `works` (
  `eid` int(11) NOT NULL,
  `did` int(11) NOT NULL,
  `pct_time` int(11) NOT NULL,
  PRIMARY KEY (`eid`,`did`),
  KEY `eid` (`eid`),
  KEY `did` (`did`),
  FOREIGN KEY (`eid`) REFERENCES `emp` (`eid`) ON DELETE CASCADE,
  FOREIGN KEY (`did`) REFERENCES `dept` (`did`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `works`
--

INSERT INTO `works` VALUES (110183,3,70),(110183,8,15),(110183,5,15),(110303,4,11),(110511,5,19),(111400,8,45),(110303,5,17),(111692,9,100),(403353,4,10), (12110, 8, 77), (12100, 8, 100), (12030, 8, 80), (12020, 8, 90), (12010, 8, 50), (12000, 8, 100);
