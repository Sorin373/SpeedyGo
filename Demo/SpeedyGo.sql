CREATE DATABASE  IF NOT EXISTS `speedygo` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `speedygo`;
-- MySQL dump 10.13  Distrib 8.0.34, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: speedygo
-- ------------------------------------------------------
-- Server version	8.0.34

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `depozit`
--

DROP TABLE IF EXISTS `depozit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `depozit` (
  `ID_Produs` int DEFAULT NULL,
  `ID_Oras` int DEFAULT NULL,
  `Cantitate_Produs` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `depozit`
--

LOCK TABLES `depozit` WRITE;
/*!40000 ALTER TABLE `depozit` DISABLE KEYS */;
INSERT INTO `depozit` VALUES (16,7,28),(18,1,2),(9,2,15),(9,6,0),(10,5,30),(17,5,27),(6,0,34),(16,9,16),(11,1,8),(16,10,19),(17,0,26),(14,3,11),(14,5,39),(1,7,6),(19,9,23),(4,5,25),(17,9,3),(11,8,29),(10,2,19),(0,5,20),(13,7,21),(19,8,7),(6,3,7),(4,1,19),(2,4,4),(12,4,0),(7,0,39),(16,6,17),(8,9,32),(7,2,18),(16,0,10),(14,6,14),(8,7,22),(0,2,9),(7,3,44),(19,5,45),(13,9,31),(4,9,49),(1,9,41),(17,6,17),(0,6,11),(0,4,45),(12,2,40),(14,10,32),(18,5,34),(17,1,8),(6,8,13),(5,7,45),(9,10,13),(1,8,4),(11,2,0),(8,1,36),(8,5,27),(15,7,46),(11,0,29),(18,0,12),(11,7,9),(2,9,14),(3,1,9),(9,3,10),(6,5,40),(19,7,7),(1,4,32),(8,4,3),(10,4,6),(9,9,35),(4,10,18),(13,3,16),(4,7,33),(5,10,1),(16,5,21),(5,2,37),(18,3,13),(3,3,15),(7,9,40),(16,4,24),(18,9,43),(19,2,26),(2,0,46),(7,7,6),(9,7,0),(7,1,21),(16,2,0),(14,8,0),(14,7,14),(7,4,4),(3,8,14),(5,8,28),(5,4,9),(17,7,46),(0,8,47),(9,8,37),(11,3,20),(6,1,41),(16,1,43),(3,5,49),(7,6,32),(1,2,36),(1,5,27),(18,7,11),(8,2,14),(2,10,25),(19,6,8),(17,8,3),(18,6,38),(19,10,33),(10,6,7),(0,0,28),(3,0,32),(4,2,46),(1,10,40),(12,0,46),(6,7,18),(7,5,46),(18,2,42),(12,6,10),(17,4,20),(10,8,20),(5,5,49),(1,3,30),(14,2,21),(2,1,21),(14,0,11),(19,0,17),(3,4,13),(5,6,6),(19,4,25),(3,7,4),(15,1,13),(11,10,48),(12,3,30),(7,8,11),(0,1,29),(11,6,30),(8,8,1),(15,9,46),(15,10,34),(13,4,47),(1,6,36),(15,4,37),(0,7,4),(9,0,2),(2,2,18),(8,10,20),(16,3,43),(9,5,3),(6,2,3),(15,0,33),(6,10,41),(11,5,20),(15,6,38),(2,8,34),(19,1,34),(2,7,30),(11,4,36),(8,6,50),(13,10,5),(19,3,12),(9,1,17),(4,8,19),(17,10,3),(10,10,1),(0,9,4),(10,1,16),(12,1,44),(13,2,21),(2,3,7),(13,5,11),(5,0,13),(11,9,48),(3,9,10),(17,3,48),(4,3,39),(1,1,11),(18,4,44),(8,0,8),(18,8,50),(16,8,3),(15,3,38),(10,0,28),(10,9,38),(5,1,14),(6,4,19),(4,0,28),(6,9,35),(13,6,12),(4,4,46),(15,8,30),(13,0,5),(18,10,43),(5,9,1),(0,10,45),(2,6,17),(13,8,44),(12,10,48),(12,8,18),(14,1,29),(15,2,16),(1,0,17),(10,7,16),(0,3,5),(12,9,14),(15,5,2),(3,6,39),(10,3,42),(3,2,38),(2,5,25),(14,9,17),(17,2,33),(6,6,36),(13,1,45),(7,10,26),(8,3,42),(5,3,41),(14,4,36),(9,4,19),(12,5,26),(3,10,20),(12,7,43),(4,6,34);
/*!40000 ALTER TABLE `depozit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `oras`
--

DROP TABLE IF EXISTS `oras`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `oras` (
  `ID_Oras` int DEFAULT NULL,
  `Denumire_Oras` text,
  `latitudine` double DEFAULT NULL,
  `longitudine` double DEFAULT NULL,
  `Tip_Depozit` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `oras`
--

LOCK TABLES `oras` WRITE;
/*!40000 ALTER TABLE `oras` DISABLE KEYS */;
INSERT INTO `oras` VALUES (0,'Brasov',45.6579752,25.6011985,'local'),(1,'Bucuresti',44.4267674,26.1025384,'centralizat'),(2,'Cluj',46.7712101,23.6236359,'local'),(3,'Constanta',44.1760175,28.6155619,'local'),(4,'Pitesti',44.8561453,24.8698545,'local'),(5,'Ramnicu Valcea',45.0999934,24.3651926,'local'),(6,'Sibiu',45.7979713,24.1374603,'centralizat'),(7,'Suceava',47.6443858,26.2575613,'local'),(8,'Targu Mures',46.5546502,24.5620975,'local'),(9,'Timisoara',45.7488716,21.2086793,'local'),(10,'Iasi',47.15984,27.5877532,'local');
/*!40000 ALTER TABLE `oras` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `produs`
--

DROP TABLE IF EXISTS `produs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `produs` (
  `ID_Produs` int DEFAULT NULL,
  `Denumire_Produs` text,
  `Categorie_Produs` text,
  `Pret_Produs` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `produs`
--

LOCK TABLES `produs` WRITE;
/*!40000 ALTER TABLE `produs` DISABLE KEYS */;
INSERT INTO `produs` VALUES (0,'Apa plata','nealcoolice',2.5),(1,'Suc de portocale','nealcoolice',3),(2,'Cola','nealcoolice',2),(3,'Cafea','nealcoolice',4.5),(4,'Ceai verde','nealcoolice',3.5),(5,'Bere blonda','alcoolice',5),(6,'Vin rosu','alcoolice',12),(7,'Vodca','alcoolice',20),(8,'Whisky','alcoolice',35),(9,'Lichior de ciocolata','alcoolice',18),(10,'Suc de mere','nealcoolice',2.8),(11,'Limonada','nealcoolice',2.5),(12,'Ceai de fructe','nealcoolice',3.2),(13,'Vin alb','alcoolice',10.5),(14,'Bere neagra','alcoolice',6.5),(15,'Gin','alcoolice',25),(16,'Rom','alcoolice',22),(17,'Vodca aromata','alcoolice',21),(18,'Apa minerala','nealcoolice',2.3),(19,'Suc de ananas','nealcoolice',3.2);
/*!40000 ALTER TABLE `produs` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-08-28 20:44:16
