VERSION := $(shell cat VERSION)

build:
	sampctl package build --forceEnsure

run:
	sampctl package run --forceBuild --container

release:
	git tag $(VERSION)
	git push
	git push origin $(VERSION)
