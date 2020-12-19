# 326.212 Final Project

2019년 전산통계 및 실습 기말 프로젝트입니다.
프로젝트를 수행하시기 전에 아래 내용을 필히 숙지하시길 바랍니다.
[Final Project FAQ](https://github.com/orgs/snu-stat/teams/sc19/discussions/1) 또한 참고해주세요.

## 뼈대코드

프로젝트를 위해 뼈대코드를 제공합니다. `part1-1.Rmd`, `flightdata.R`, `part2.Rmd`, `part3.Rmd` 파일을 고치시면 됩니다.

- `data/`: 자료 폴더입니다.
    로컬 상에서 `.csv` 파일과 년도별 `.zip` 파일들을 이 폴더 안에 다운로드 해주신 다음 프로젝트를 진행하시길 바랍니다.
- `part1-1.Rmd`: Part 1-1 문제의 코드를 이곳에 적고 html을 knit해주세요.
- `flightdata.R`: Part 1의 나머지 문제를 여기에 푸시면 됩니다.
    `source`를 했을 때, `project.sqlite` 이름의 데이터베이스가 만들어져야 합니다.
- `part2.Rmd`: Part 2 문제를 여기에 푼 뒤, html을 knit해주세요.
- `part3.Rmd`: Part 3 문제를 여기에 푼 뒤, html을 knit해주세요.

## 주의사항

- **`.zip`, `.csv`, `.csv.bz2`, `.sqlite`와 같은 데이터 파일들을 절대 커밋하지 마세요.**
- **`.gitignore` 파일을 삭제하지 마세요.**
- **최종 커밋 시 처음 주어진 파일과 이름이 다른 파일이 GitHub 상에 올라가 있으면 안됩니다. `master` 브랜치의 모든 파일의 이름이 `project` 브랜치와 같은 이름으로 되어 있는지를 꼭 확인해주세요.**
