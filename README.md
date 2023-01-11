# hrc_pomdp

## About the project
...

### Citation
This project started in the context of Ph.D. research into _intention-based human-robot collaborative assembly_, and was first mentioned in the following paper:
> M. Cramer, K. Kellens and E. Demeester, "Probabilistic Decision Model for Adaptive Task Planning in Human-Robot Collaborative Assembly Based on Designer and Operator Intents," in IEEE Robotics and Automation Letters, vol. 6, no. 4, pp. 7325-7332, Oct. 2021, doi: 10.1109/LRA.2021.3095513.


## Getting started
These instructions will help you get the presented software up and running on your local machine.

### Dependencies
The third-party libraries used in this project are acquired and managed by the open source C/C++ dependency manager 'vcpkg'. Instructions about how to install this package manager can be found on vcpkg's [website](https://vcpkg.io/en/getting-started.html).

This project relies on the following external C++ libraries:
* [boost](https://www.boost.org/)
* [tinyxml2](https://github.com/leethomason/tinyxml2)
* [graphviz](https://graphviz.org/)

... that can be installed by executing the next commands:
```shell
$ cd <path-to-src-directory-of-vcpkg>
$ ./vcpkg install <packages-to-install>
```

## License
This project is distributed under the GPL-3.0 License. See `LICENSE` for more information.


## Acknowledgments
* [KU Leuven](https://iiw.kuleuven.be/english/diepenbeek) @ Diepenbeek Campus
* Automation, Computer vision and Robotics ([ACRO](https://iiw.kuleuven.be/onderzoek/acro)) research unit
* Research Foundation - Flanders ([FWO](https://www.fwo.be/en/))
