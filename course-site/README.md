# minicc Course Site

`course-site/` 是 `minicc` 课程讲义的静态网站外壳。

内容真源仍然是仓库根下的 `course/`。本目录只负责：

- 把 `course/**/*.md` 同步成可发布的 Jekyll 页面
- 生成课程侧边栏导航
- 生成一页式连续阅读页
- 本地预览与静态构建

## 本地预览

1. 安装 Ruby 和 Bundler
2. 在本目录执行：

```bash
bundle config set path vendor/bundle
bundle install
python3 _scripts/sync_course.py
bundle exec jekyll serve
```

3. 打开 <http://localhost:4000>

## 目录约定

- `../course/`：课程内容真源
- `pages/_course/`：由同步脚本生成的 Jekyll collection 页面
- `_data/navigation.yml`：由同步脚本生成的侧边栏导航
- `pages/_course/continuous.md`：由同步脚本生成的一页式连续阅读页

## 更新课程内容后的发布前动作

```bash
python3 _scripts/sync_course.py
bundle exec jekyll build
```
