from setuptools import setup, find_packages
import os


def find_stubs(package):
    stubs = []
    for root, dirs, files in os.walk(package):
        for file in files:
            path = os.path.join(root, file).replace(package + os.sep, '', 1)
            stubs.append(path)
    return dict(package=stubs)


setup(
    name='spylizard_stubs',
    maintainer="spylizard Developers",
    maintainer_email="example@python.org",
    description="PEP 561 type stubs for spylizard",
    version='1.0',
    packages=['spylizard_stubs'],
    package_dir={'spylizard_stubs':'.'},
    include_package_data = True,
    # PEP 561 requires these
    package_data={"": ["*.py"]},
)