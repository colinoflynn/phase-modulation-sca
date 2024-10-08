# T-CHES Artifact

The T-CHES Artifact is part of the main [GITHub repository](https://github.com/colinoflynn/phase-modulation-sca). It is specifically seen in the `tches_artifact` folder.

The uploaded file for the artifact contains the root directory, which contains many *additional* results beyond the core artifact which recreates the most important parts of the results.

This folder contains a "artifact" for the paper, that more directly recreates the results. This artifact has the same information as the Jupyter Notebooks, but is designed for a "single run" that can be used to more quickly verify the results.

## Reproducable Results

The following results in the paper will be reproduced using the dataset:

* Table 1 (Baseline Shunt-based CPA & T-Test Results)
* Table 2 (Mixer-based PM Results using Optical & IC-Based Isolator Boards)
* Table 3 (I/O Pin Leakage T-Test Results)
* Table 4 (JTAG Phase Modulation CPA & T-Test Resutls)
* Table 7 (JTAG Fault Correlation Analysis CPA & T-Test Results)

The notebooks can additionally recreate most of the figures. For the purpose of this artifact the tables which prove the important results are the main goal of this artifact.


## Running the Artifact(s)

The artifacts have been optimized to run on Google Colab, but can run on any local Jupyter install. If using a local Jupyter install you have more flexability, due to disk size limits the artifacts split up the runs into smaller chunks.

The notebook itself should install any non-default packages that are required.

To run them on Google Colab, simply upload & run each notebook. Each notebook describes the expected recreateable results, which should match the paper.

These notebooks use recorded power traces. See below for capturing new power traces.

Artifact has been run on Python 3.10 (tested on 3.10.8 and 3.10.12), with the `requirements.txt` from the 3.10.12 install.

## Recreating Hardware Captures

The repository also contains the hardware and scripts for recreating the captures. As this requires special hardware has been left out of the artifact folder, but is part of the main repository.

The scripts for capturing associated datasets can also be run to recreate more parts of the paper. For the T-CHES artifact evaluation process this hardware can be provided to an evaluator, see the [notebooks_capture](https://github.com/colinoflynn/phase-modulation-sca/tree/main/notebooks_capture) directory of the repository.