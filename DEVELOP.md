## 开发流程

intorobot firmware 版本分支包括如下:
- master     最新发布分支
- develop    开发分支
- release/*  发布分支
- feature/*  特定功能开发分支

开发过程原则:
- master/develop/release分支不允许提交。 develop只允许通过pull request进行提交修改。
1. 如果有新功能或者缺陷. 从develop分支创建一个新的feature分支.feature开发完毕后，发起pull request合并到develop分支。这个过程需要代码review.
2. 新功能和缺陷通过github issure进行管理。
