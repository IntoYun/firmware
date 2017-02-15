## 发布流程
    功能测试完毕，进入发布流程。具体操作如下

1. 确认固件库版本号.     /build/version.mk和/release/release-version.sh
2. 生成在线编程工程包.   指令: ./release-online-program.sh all
3. 生成模块完整下载包.   指令: ./release-package.sh all
4. 上传在线编程工具包.   指令: ./release-publish.sh intorobot
5. 创建一个tag.          指令: git tag -a v1.4.3  运行后填写tag信息说明。tag信息说明从CHANGELOG.md复制
6. 推送tag至github.      指令: git push origin --tags
7. 创建一个发布版本分支. 指令: git branch release/v1.4.3
8. 推送发布分支到服务器. 指令: git push origin release/v1.4.3
9. 登陆github. 填写release发布信息和上传压缩文件. 发布信息从CHANGELOG.md复制.
