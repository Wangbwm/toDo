/*
 Navicat Premium Data Transfer

 Source Server         : mysql-114.132.229.115
 Source Server Type    : MySQL
 Source Server Version : 80032
 Source Host           : 114.132.229.115:3306
 Source Schema         : todo

 Target Server Type    : MySQL
 Target Server Version : 80032
 File Encoding         : 65001

 Date: 14/05/2023 22:48:11
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for UserAccount
-- ----------------------------
DROP TABLE IF EXISTS `UserAccount`;
CREATE TABLE `UserAccount`  (
  `id` int(0) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `password` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `username`(`username`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of UserAccount
-- ----------------------------
INSERT INTO `UserAccount` VALUES (1, 'admin', '21232f297a57a5a743894a0e4a801fc3');
INSERT INTO `UserAccount` VALUES (3, 'wangbw', 'ac0eaf809a53a0f7cb2d7c519b5e625d');

-- ----------------------------
-- Table structure for todo_groups
-- ----------------------------
DROP TABLE IF EXISTS `todo_groups`;
CREATE TABLE `todo_groups`  (
  `id` int(0) NOT NULL AUTO_INCREMENT,
  `group_name` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `member_ids` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `group_name`(`group_name`(255)) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of todo_groups
-- ----------------------------
INSERT INTO `todo_groups` VALUES (1, '金山', '1');
INSERT INTO `todo_groups` VALUES (12, '测试多人分组', '2');
INSERT INTO `todo_groups` VALUES (15, '测试wangbw邀请admin 1', '2,1');
INSERT INTO `todo_groups` VALUES (16, '测试wangbw邀请admin 2', '2');
INSERT INTO `todo_groups` VALUES (17, '测试admin邀请wangbw 1', '1,2');
INSERT INTO `todo_groups` VALUES (18, '测试修改名称', '1,2');

-- ----------------------------
-- Table structure for todo_info
-- ----------------------------
DROP TABLE IF EXISTS `todo_info`;
CREATE TABLE `todo_info`  (
  `id` int(0) NOT NULL AUTO_INCREMENT,
  `user_id` int(0) NOT NULL,
  `group_id` int(0) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `user_id`(`user_id`) USING BTREE,
  INDEX `group_id`(`group_id`) USING BTREE,
  CONSTRAINT `todo_info_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `todo_user` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `todo_info_ibfk_2` FOREIGN KEY (`group_id`) REFERENCES `todo_groups` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of todo_info
-- ----------------------------

-- ----------------------------
-- Table structure for todo_list
-- ----------------------------
DROP TABLE IF EXISTS `todo_list`;
CREATE TABLE `todo_list`  (
  `id` int(0) NOT NULL AUTO_INCREMENT,
  `user_id` int(0) NULL DEFAULT NULL,
  `group_id` int(0) NULL DEFAULT NULL,
  `event_name` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `is_arranged` tinyint(1) NOT NULL DEFAULT 0,
  `priority` int(0) NULL DEFAULT NULL,
  `due_date` datetime(0) NULL DEFAULT NULL,
  `is_repeated` tinyint(1) NULL DEFAULT NULL,
  `remind_time` datetime(0) NULL DEFAULT NULL,
  `description` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `location` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `note` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of todo_list
-- ----------------------------
INSERT INTO `todo_list` VALUES (1, 1, NULL, '大作业', 1, 3, '2023-05-26 20:00:00', 0, NULL, NULL, NULL, NULL);
INSERT INTO `todo_list` VALUES (2, NULL, 1, '我的作业', 1, 2, '2024-05-14 10:52:12', 0, '2024-05-13 10:52:12', '金山作业', '中国地质大学未来城校区', '没有备注');
INSERT INTO `todo_list` VALUES (4, 1, NULL, 'QT预习', 1, 2, '2023-05-13 01:28:02', 1, '2023-05-13 19:45:42', NULL, NULL, NULL);
INSERT INTO `todo_list` VALUES (7, 1, NULL, '测试', 0, 3, NULL, 0, NULL, NULL, NULL, NULL);
INSERT INTO `todo_list` VALUES (10, NULL, 1, '金山待办', 0, 3, NULL, 0, NULL, NULL, NULL, NULL);
INSERT INTO `todo_list` VALUES (11, NULL, 15, '待办1', 0, NULL, NULL, 0, NULL, NULL, NULL, NULL);
INSERT INTO `todo_list` VALUES (12, NULL, 16, '待办2', 0, NULL, NULL, 0, NULL, NULL, NULL, NULL);
INSERT INTO `todo_list` VALUES (15, NULL, 17, '待办', 1, 1, '2023-05-14 19:26:55', 0, NULL, NULL, NULL, '');

-- ----------------------------
-- Table structure for todo_user
-- ----------------------------
DROP TABLE IF EXISTS `todo_user`;
CREATE TABLE `todo_user`  (
  `id` int(0) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `groups_list` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of todo_user
-- ----------------------------
INSERT INTO `todo_user` VALUES (1, 'admin', '1,15,17,18');
INSERT INTO `todo_user` VALUES (2, 'wangbw', '12,15,16,17,18');

SET FOREIGN_KEY_CHECKS = 1;
