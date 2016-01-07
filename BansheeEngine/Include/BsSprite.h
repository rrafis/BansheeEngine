#pragma once

#include "BsPrerequisites.h"
#include "BsVector2I.h"
#include "BsRect2I.h"
#include "BsColor.h"

namespace BansheeEngine
{
	/** Determines position of the sprite in its bounds. */
	enum SpriteAnchor
	{
		SA_TopLeft,
		SA_TopCenter,
		SA_TopRight,
		SA_MiddleLeft,
		SA_MiddleCenter,
		SA_MiddleRight,
		SA_BottomLeft,
		SA_BottomCenter,
		SA_BottomRight
	};

	/** Types of materials available for rendering sprites. */
	enum class SpriteMaterial
	{
		Text, Image, ImageAlpha
	};

	/** Contains information for initializing a sprite material. */
	struct SpriteMaterialInfo
	{
		/** Generates a hash value that describes the contents of this object. */
		UINT64 generateHash() const;

		SpriteMaterial type;
		UINT64 groupId;
		HTexture texture;
		Color tint;
	};

	/** @cond INTERNAL */

	/** Equals operator for SpriteMaterialInfo. */
	bool operator==(const SpriteMaterialInfo& lhs, const SpriteMaterialInfo& rhs);

	/** Not equals operator for SpriteMaterialInfo. */
	bool operator!=(const SpriteMaterialInfo& lhs, const SpriteMaterialInfo& rhs);

	/** Contains information about a single sprite render element, including its geometry and material. */
	struct SpriteRenderElement
	{
		SpriteRenderElement()
			:vertices(nullptr), uvs(nullptr), indexes(nullptr), numQuads(0)
		{ }

		Vector2* vertices;
		Vector2* uvs;
		UINT32* indexes;
		UINT32 numQuads;
		SpriteMaterialInfo matInfo;
	};

	/** @endcond */

	/**
	 * @brief	Generates geometry and contains information needed for rendering
	 *			a two dimensional element.
	 */
	class BS_EXPORT Sprite
	{
	public:
		Sprite();
		virtual ~Sprite();

		/**
		 * @brief	Returns clipped bounds of the sprite.
		 *
		 * @param	offset		Offset that will be added to the returned bounds.
		 * @param	clipRect	Local clip rect that is used for clipping the sprite bounds.
		 *						(Clipping is done before the offset is applied). If clip rect
		 *						width or height is zero, no clipping is done.
		 *
		 * @returns	Clipped sprite bounds.
		 */
		Rect2I getBounds(const Vector2I& offset, const Rect2I& clipRect) const;

		/**
		 * @brief	Returns the number of separate render elements in the sprite. Normally this is 1, but some sprites
		 * 			may consist of multiple materials, in which case each will require its own mesh (render element)
		 * 			
		 * @return	The number render elements.
		 */
		UINT32 getNumRenderElements() const;

		/**
		 * @brief	Gets a material for the specified render element index.
		 * 		
		 * @return	Structure describing the material.
		 *
		 * @see		getNumRenderElements()
		 */
		const SpriteMaterialInfo& getMaterialInfo(UINT32 renderElementIdx) const;

		/**
		 * @brief	Returns the number of quads that the specified render element will use. You will need this
		 * 			value when creating the buffers before calling "fillBuffer".
		 * 			
		 * @return	Number of quads for the specified render element. 
		 *	
		 * @note	Number of vertices = Number of quads * 4
		 *			Number of indices = Number of quads * 6
		 *			
		 * @see		getNumRenderElements()
		 * @see		fillBuffer()
		 */
		UINT32 getNumQuads(UINT32 renderElementIdx) const;

		/**
		 * @brief	Fill the pre-allocated vertex, uv and index buffers with the mesh data for the
		 * 			specified render element.
		 *
		 * @param	vertices			Previously allocated buffer where to store the vertices.
		 * @param	uv					Previously allocated buffer where to store the uv coordinates.
		 * @param	indices				Previously allocated buffer where to store the indices.
		 * @param	startingQuad		At which quad should the method start filling the buffer.
		 * @param	maxNumQuads			Total number of quads the buffers were allocated for. Used only
		 * 								for memory safety.
		 * @param	vertexStride		Number of bytes between of vertices in the provided vertex and uv data.
		 * @param	indexStride			Number of bytes between two indexes in the provided index data.
		 * @param	renderElementIdx	Zero-based index of the render element.
		 *
		 * @see		getNumRenderElements()
		 * @see		getNumQuads()
		 */
		UINT32 fillBuffer(UINT8* vertices, UINT8* uv, UINT32* indices, UINT32 startingQuad, UINT32 maxNumQuads, 
			UINT32 vertexStride, UINT32 indexStride, UINT32 renderElementIdx, const Vector2I& offset, const Rect2I& clipRect, bool clip = true) const;

	protected:
		/**
		 * @brief	Clips the provided vertices to the provided clip rectangle.
		 *
		 * @param	vertices	Pointer to the start of the buffer containing vertex positions.
		 * @param	uv			Pointer to the start of the buffer containing UV coordinates.
		 * @param	numQuads	Number of quads in the provided buffer pointers.
		 * @param	vertStride	Number of bytes to skip when going to the next vertex. This assumes both position
		 *						and uv coordinates have the same stride (as they are likely pointing to the same buffer).
		 * @param	clipRect	Rectangle to clip the geometry to.
		 */
		static void clipToRect(UINT8* vertices, UINT8* uv, UINT32 numQuads, UINT32 vertStride, const Rect2I& clipRect);

		/**
		 * @brief	Returns the offset needed to move the sprite in order for it to respect the provided anchor.
		 */
		static Vector2I getAnchorOffset(SpriteAnchor anchor, UINT32 width, UINT32 height);

		/**
		 * @brief	Calculates the bounds of all sprite vertices.
		 */
		void updateBounds() const;

		mutable Rect2I mBounds;
		mutable Vector<SpriteRenderElement> mCachedRenderElements;
	};
}

/** @cond STDLIB */
/** @addtogroup GUI
 *  @{
 */

/**	Hash value generator for STL reference wrapper for SpriteMaterialInfo. */
template<>
struct std::hash<std::reference_wrapper<const BansheeEngine::SpriteMaterialInfo>>
{
	size_t operator()(const std::reference_wrapper<const BansheeEngine::SpriteMaterialInfo>& value) const
	{
		return (size_t)value.get().generateHash();
	}
};

/** Provides equals operator for STL reference wrapper for SpriteMaterialInfo. */
static bool operator==(const std::reference_wrapper<const BansheeEngine::SpriteMaterialInfo>& lhs, 
	const std::reference_wrapper<const BansheeEngine::SpriteMaterialInfo>& rhs)
{
	return lhs.get() == rhs.get();
}

/** Provides not equals operator for STL reference wrapper for SpriteMaterialInfo. */
static bool operator!=(const std::reference_wrapper<const BansheeEngine::SpriteMaterialInfo>& lhs,
	const std::reference_wrapper<const BansheeEngine::SpriteMaterialInfo>& rhs)
{
	return !(lhs == rhs);
}

/** @} */
/** @endcond */