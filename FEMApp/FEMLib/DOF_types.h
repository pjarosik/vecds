
#pragma once



enum DOF_TYPES
{
	DOF_UX, /*!< Displacement x DOF type. */
	DOF_UY, /*!< Displacement y DOF type  */
	DOF_UZ, /*!< Displacement x DOF type  */
	DOF_FIX, /*!< Rotation with respect to X axis */
	DOF_FIY, /*!< Rotation with respect to Y axis  */
	DOF_FIZ, /*!< Rotation with respect to Z axis  */
	DOF_T,  /*!< temperature  */
	DOF_D,  /*!< diffusion  */
    DOF_C,  /*!< chemistry  */
	DOF_AX, /*!< Heavisite xfem DOF type  */
	DOF_AY, /*!< Heavisite xfem DOF type  */
	DOF_BX, /*!< near-tip  xfem DOF type  */
	DOF_BY, /*!< near-tip  xfem DOF type  */
};
