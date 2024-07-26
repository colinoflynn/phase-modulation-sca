# T-CHES Artifact

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

## Recreating Hardware Captures

The repository also contains the hardware and scripts for recreating the captures. As this requires special hardware has been left out of the artifact folder, but is part of the main repository.

The scripts for capturing associated datasets can also be run to recreate more parts of the paper. For the T-CHES artifact evaluation process this hardware can be provided to an evaluator, see the [notebooks_capture](https://github.com/colinoflynn/phase-modulation-sca/tree/main/notebooks_capture) directory of the repository.