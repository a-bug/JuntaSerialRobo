from setuptools import setup

package_name = 'pyconvo'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='aluno',
    maintainer_email='mic.weiss.hael@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'logger = pyconvo.logger:main',
            'convo = pyconvo.convo:main',
            'talker = pyconvo.talker:main'
        ],
    },
)
