-- MySQL dump 10.13  Distrib 5.6.24, for Win32 (x86)
--
-- Host: localhost    Database: r1-14
-- ------------------------------------------------------
-- Server version	5.6.26-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `people`
--

DROP TABLE IF EXISTS `people`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `people` (
  `idpeople` int(11) NOT NULL,
  `peoplename` varchar(45) DEFAULT NULL,
  `peoplesex` varchar(45) DEFAULT NULL,
  `peopledegree` varchar(45) DEFAULT NULL,
  `peoplertimetoday` varchar(45) DEFAULT NULL,
  `peopleweektoday` varchar(45) DEFAULT NULL,
  `peopleworkbegin` varchar(45) DEFAULT NULL,
  `peopleworkafter` varchar(45) DEFAULT NULL,
  `peopletimetotal` varchar(45) DEFAULT NULL,
  `peopleshouldworktime` varchar(45) DEFAULT NULL,
  `peopletodayworktime` varchar(45) DEFAULT NULL,
  `peopleipaddress` varchar(45) DEFAULT NULL,
  `peopletel` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idpeople`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `people`
--

LOCK TABLES `people` WRITE;
/*!40000 ALTER TABLE `people` DISABLE KEYS */;
INSERT INTO `people` VALUES (1,'夏菠','男','研三','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.150','15681204026'),(2,'余亚玲','女','研三','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.151','15284048309'),(3,'李培培','女','研二','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.152','15181446068'),(4,'任俊松','男','研二','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.153','15528589808'),(5,'赵浩','男','研二','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.154','15281671634'),(6,'汪双','男','研二','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.155','15328114945'),(7,'赵建强','男','研二','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.156','18009050283'),(8,'邱恒','男','研二','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.157','13518303428'),(9,'杨帆','男','大四','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.158','00000000000'),(10,'刘老师','男','博士','2015-9-4','星期一','8：30','17：30','0','40','9','192.168.1.159','13547127010');
/*!40000 ALTER TABLE `people` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-09-06 16:13:09
